#include <QDebug>
#include "CMqttClientHandler.h"
#include <QtMqtt/qmqttclient.h>
#include <QtCore/QTimer>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>
#include <QMap>

namespace
{
    const int RECONNECT_TIMEOUT{ 5 * 1000 };
    const int FAST_WAITING_TIMEOUT{ 100 };
    const int NORM_WAITING_TIMEOUT{ 5000 };
    const int MAXIMUM_QUEUE_SIZE{ 10000 };
    const QMap<QAbstractSocket::SocketError, QString> descriptions =
    {{QAbstractSocket::ConnectionRefusedError, "The connection was refused by the peer (or timed out)."},
     {QAbstractSocket::RemoteHostClosedError, "The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent."},
     {QAbstractSocket::HostNotFoundError, "The host address was not found."},
     {QAbstractSocket::SocketAccessError, "The socket operation failed because the application lacked the required privileges."},
     {QAbstractSocket::SocketResourceError, "The local system ran out of resources (e.g., too many sockets)."},
     {QAbstractSocket::SocketTimeoutError, "The socket operation timed out."},
     {QAbstractSocket::DatagramTooLargeError, "The datagram was larger than the operating system's limit (which can be as low as 8192 bytes)."},
     {QAbstractSocket::NetworkError, "An error occurred with the network (e.g., the network cable was accidentally plugged out)."},
     {QAbstractSocket::AddressInUseError, "The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive."},
     {QAbstractSocket::SocketAddressNotAvailableError, "The address specified to QAbstractSocket::bind() does not belong to the host."},
     {QAbstractSocket::UnsupportedSocketOperationError, "The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support)."},
     {QAbstractSocket::ProxyAuthenticationRequiredError, "The socket is using a proxy, and the proxy requires authentication."},
     {QAbstractSocket::SslHandshakeFailedError, "The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)"},
     {QAbstractSocket::UnfinishedSocketOperationError, "Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background)."},
     {QAbstractSocket::ProxyConnectionRefusedError,   "Could not contact the proxy server because the connection to that server was denied"},
     {QAbstractSocket::ProxyConnectionClosedError, "The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)"},
     {QAbstractSocket::ProxyConnectionTimeoutError, "The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase."},
     {QAbstractSocket::ProxyNotFoundError, "The proxy address set with setProxy() (or the application proxy) was not found."},
     {QAbstractSocket::ProxyProtocolError, "The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood."},
     {QAbstractSocket::OperationError, "An operation was attempted while the socket was in a state that did not permit it."},
     {QAbstractSocket::SslInternalError, "The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library."},
     {QAbstractSocket::SslInvalidUserDataError, "Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library."},
     {QAbstractSocket::TemporaryError, "A temporary error occurred (e.g., operation would block and socket is non-blocking)."},
     {QAbstractSocket::UnknownSocketError, "An unidentified error occurred."}
    };
    const QMap<CMqttClientHandler::ClientState, QString> states =
    {{CMqttClientHandler::ClientState::Connected, "Connected"},
     {CMqttClientHandler::ClientState::Disconnected, "Disconnected"},
     {CMqttClientHandler::ClientState::Connecting, "Connecting"}};
}

/** ****************************************************************************
 * @fn CMqttClientHandler  [конструктор]
 ** ***************************************************************************/
CMqttClientHandler::CMqttClientHandler(QObject * parent,
        const QString &clientName) :
        QObject(parent),
        txMessageTimeoutTimer(nullptr)
        /*CMqttClientHandler(CMqttConnectionSettings(), parent, clientName)*/
{
    init(clientName);
}

CMqttClientHandler::CMqttClientHandler(
        const CMqttConnectionSettings &settings,
        QObject * parent,
        const QString &clientName) :
        QObject(parent),
        m_settings(settings.getIp(), settings.getPort()),
        txMessageTimeoutTimer(nullptr)
        /*,
        m_clientName(clientName),
        txMessageTimeoutTimer(new QTimer(this))
*/
{
    init(clientName);
}

void CMqttClientHandler::init(const QString &clientName)
{
    m_client = nullptr;
    m_connectTimeoutTimer = nullptr;
    m_clientName = clientName;
    txMessageTimeoutTimer = new QTimer(this);
    /*static auto id1 = */qRegisterMetaType<ClientState>("ClientState");
    /*static auto id2 = */qRegisterMetaType<SubscriptionState>("SubscriptionState");
    /*static auto id3 = */qRegisterMetaType<CMqttMessage>("CMqttMessage");
/*    Q_UNUSED( id1 );
    Q_UNUSED( id2 );
    Q_UNUSED( id3 );
*/
    connect(txMessageTimeoutTimer, SIGNAL(timeout()), this,
            SLOT(sendMessageTimeout()), Qt::AutoConnection);

    m_queue.reserve(MAXIMUM_QUEUE_SIZE);
    connect( this, & CMqttClientHandler::destroyed, this,
             &CMqttClientHandler::threadStopedSlot );
}

CMqttClientHandler::~CMqttClientHandler()
{
    if(txMessageTimeoutTimer)
    {
        delete txMessageTimeoutTimer;
    }

    if(m_client)
    {
        m_client->disconnectFromHost();
        delete m_client;
    }

    if(m_connectTimeoutTimer)
    {
        m_connectTimeoutTimer->stop();
        delete m_connectTimeoutTimer;
    }
}


/** ****************************************************************************
 * @fn reconnectTimerSlot
 * @brief слот, выполняет переподключение к брокеру
 * @param none
 * @return none
 ** ***************************************************************************/
void CMqttClientHandler::printMessage(const QString &message, CLoggerMessage::ELogCriteria criteria,
        bool printLogMessages)
{
    if(printLogMessages) {
        PRINT_LOG_MESSAGE(message, criteria, false );
    }
    switch(criteria) {
    case CLoggerMessage::ELogCriteria::LogCriteria_info:
        PRINT_CONSOLE_MESSAGE_INFO(message);
        break;
    case CLoggerMessage::ELogCriteria::LogCriteria_warning:
        PRINT_CONSOLE_MESSAGE_WARN(message);
        break;
    case CLoggerMessage::ELogCriteria::LogCriteria_error:
        PRINT_CONSOLE_MESSAGE_ERROR(message);
        break;
    }
}

/*
const QMqttClient *CMqttClientHandler::getMqttClient()
{
    return m_client;
}
*/

void CMqttClientHandler::reconnectSlot()
{
    if ( ! m_client )
    {
        printMessage(QString( "Объект MQTT клиента не "
                              "проинициализирован! Прежде выполните слот "
                                                            "[threadStartedSlot]" ),
                     CLoggerMessage::ELogCriteria::LogCriteria_error,
                     m_printLogMessages);
        return;
    }
    const ClientState state{ static_cast< ClientState >( m_client->state() ) };
    if ( state == ClientState::Disconnected )
    {
        if ( ! m_settings.isValid() )
        {
            printMessage(QString("Некорректные настройки "
                                 "подключения к брокеру MQTT [%1] - проверьте настройки!").
                                                          arg( m_settings.getLastError()),
                    CLoggerMessage::ELogCriteria::LogCriteria_error,
                    m_printLogMessages);
        }
        else
        {
            if(this->clientName()!="") {
                 printMessage(QString("Попытка подключения "
                "клиента %1 к брокеру MQTT IP=%2, port=%3 из состояния "
                                                     "\"disconnected\"").
                                         arg(this->clientName()).
                                            arg(m_settings.getIp()).
                                         arg(m_settings.getPort()),
                              CLoggerMessage::ELogCriteria::LogCriteria_info,
                              m_printLogMessages);
            }
            m_client->setHostname( m_settings.getIp() );
            m_client->setPort( m_settings.getPort() );
            m_connectTimeoutTimer->start();
            // ????????????????????????????????????????????

            //abstractSocket->connectToHost(m_settings.getIp(),
            //        m_settings.getPort(), QAbstractSocket::OpenModeFlag::ReadWrite);
            // ??????????????????????????????????????????
            m_client->connectToHost();
            // Соединяем сигналы нашего транспорта для большего контроля над состоянием
            // сокета
            QAbstractSocket *abstractSocket = qobject_cast<QAbstractSocket*>
                    (m_client->transport());
            if(abstractSocket!=nullptr) {
                connect(abstractSocket, static_cast<void(QAbstractSocket::*)
                        (QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                        [this](QAbstractSocket::SocketError socketError)
                {

                    QString errDescr = descriptions.contains(socketError)?
                                descriptions[socketError]: "Unknown error";

                    printMessage(QString("ошибка TCP socketa "
                                                        "MQTT клиента %1: %2").arg(this->clientName()).
                                                arg(errDescr),
                                 CLoggerMessage::ELogCriteria::LogCriteria_error,
                                 m_printLogMessages);
                    //const ClientState state{ static_cast< ClientState >( m_client->state() ) };

                    //reconnectSlot();
                });

                // ?????????????? Для отладки???????????????????????????????????
                /*connect(abstractSocket, &QAbstractSocket::hostFound, this, [this]()
                {
                    printMessage(QString("Host found signal emitted"),
                                 CLoggerMessage::ELogCriteria::LogCriteria_error,
                                 m_printLogMessages);
                });*/
                /*connect(abstractSocket, &QAbstractSocket::stateChanged, this, [this](QAbstractSocket::SocketState socketState)
                {
                    printMessage(QString("stateChanged signal for TCP socket emitted state=%1").arg(socketState),
                                 CLoggerMessage::ELogCriteria::LogCriteria_error,
                                 m_printLogMessages);
                });*/

                /*connect(abstractSocket, &QAbstractSocket::proxyAuthenticationRequired, this,
                    [](const QNetworkProxy &proxy, QAuthenticator *authenticator)
            {
                printMessage(QString("proxyAuthenticationRequired"),
                CLoggerMessage::ELogCriteria::LogCriteria_error);
            });*/

                // ?????????????????????????????????????????????????????????
            }
        }
    }
    else if ( state == ClientState::Connecting )
    {
        if ( QAbstractSocket * socket = qobject_cast< QAbstractSocket * >
                ( m_client->transport() ) )
        {
            socket->waitForConnected( 1 );
            socket->blockSignals( true );
            m_client->disconnectFromHost();
            socket->blockSignals( false );
            //m_client->setState( QMqttClient::Disconnected );
            if ( ! m_settings.isValid() )
            {
                printMessage( QString("Некорректные настройки "
                        "подключения к брокеру MQTT [%1] - проверьте "
                        "настройки!").arg( m_settings.getLastError() ),
                        CLoggerMessage::ELogCriteria::LogCriteria_error,
                        m_printLogMessages);
            }
            else
            {
                if(this->clientName()!="") {
                    printMessage( QString("Попытка подключения "
                    "клиента %1 к брокеру MQTT IP=%2, port=%3 из состояния "
                                                         "\"Connecting\"").
                                             arg(this->clientName()).
                                             arg(m_settings.getIp()).
                                             arg(m_settings.getPort()),
                            CLoggerMessage::ELogCriteria::LogCriteria_info,
                            m_printLogMessages);
                }
                m_client->setHostname( m_settings.getIp() );
                m_client->setPort( m_settings.getPort() );
                m_connectTimeoutTimer->start();
                m_client->connectToHost();
            }
        }
        else
        {
            printMessage(  "Некорректный транспортный QAbstractSocket!" ,
                           CLoggerMessage::ELogCriteria::LogCriteria_error,
                           m_printLogMessages);
        }
    }
    else
    {
        forceDisconnectSlot();
        reconnectSlot();
    }

}

/** ****************************************************************************
 * @fn forceDisconnectSlot
 * @brief слот, получает задание на отключение от брокера
 * @param none
 * @return none
 ** ***************************************************************************/
bool CMqttClientHandler::forceDisconnectSlot()
{
    bool res{ false };
    if ( ! m_client )
    {
        printMessage( QString( "Объект MQTT клиента не "
                "проинициализирован! Прежде выполните слот "
                "[threadStartedSlot]" ),
                CLoggerMessage::ELogCriteria::LogCriteria_error,
                m_printLogMessages);
    }
    else
    {
        m_passReconnect = true;
        const ClientState state{ static_cast< ClientState >( m_client->state() ) };
        if ( state == ClientState::Disconnected )
        {

        }
        else if ( state == ClientState::Connecting )
        {
            if ( QAbstractSocket * socket = qobject_cast< QAbstractSocket * >( m_client->transport() ) )
            {
                socket->waitForConnected( 1 );
                socket->blockSignals( true );
                m_client->disconnectFromHost();
                socket->blockSignals( false );
                m_client->setState( QMqttClient::Disconnected );
            }
            else
            {
                 printMessage( "Некорректный транспортный QAbstractSocket!" ,
                            CLoggerMessage::ELogCriteria::LogCriteria_error,
                            m_printLogMessages);
            }
        }
        else
        {
            m_client->disconnectFromHost();
        }
        res = true;
    }
    emit forceDisconnectResult( res, QPrivateSignal{} );
    return res;
}

/** ****************************************************************************
 * @fn threadStart
 * @brief слот, вызывается после переноса объекта в нужный поток и его запуска.
 * Если объект не будет переноситься в отдельный поток - данный слот запускать
 * сразу
 * @param none
 * @return none
 ** ***************************************************************************/
void CMqttClientHandler::threadStartedSlot()
{
    m_client = new QMqttClient(this);
    //abstractSocket = new QTcpSocket(this); // Создали транспорт
                                        // для клиента
    //m_client->setTransport(abstractSocket,
    //                       QMqttClient::TransportType::AbstractSocket);



    //
    connect( m_client, &QMqttClient::disconnected, this, [ this ]()
    {
        if ( ! m_passReconnect )
        {
            m_connectTimeoutTimer->start();//            reconnectSlot();
        }
        else
        {
            m_passReconnect = false;
        }
        emit disconnected();
    }, Qt::AutoConnection);
    connect( m_client, &QMqttClient::connected, this, [ this ]()
    {
        //printMessage( QString( "keepAlive=%1" ).arg(m_client->keepAlive()),
        //        CLoggerMessage::ELogCriteria::LogCriteria_info);
        m_connectTimeoutTimer->stop();
        // Восстанавливаем подписки на топики
        for(auto it = m_subscrubedTopics.begin(); it!=m_subscrubedTopics.end();
                it++) {
            SubscriptionState state = subscribeSlot(it.key(), it.value());
            if(state != SubscriptionState::Subscribed && state !=
                    SubscriptionState::SubscriptionPending) {
                printMessage(  QString( "Клиенту MQTT "
                        "не удалось подписаться на топик %1 после реконнекта").
                        arg(it.key()),
                        CLoggerMessage::ELogCriteria::LogCriteria_error,
                        m_printLogMessages);
            }
        }
        // Добавляем retain сообщения, которые были
        // опубликованы до реконекта, для этого добавляем их в голову
        // очереди (чтобы их затем затерли неопубликованные сообщения
        // из очереди, если очередь содержит те же топики)
        for(const auto &mes: m_retainMessages) {
            m_queue.push_front(mes);
        }
        // Добавляем в голову очереди сообщения, которые были
        // отправлены, но подтверждения о их приеме не получено
        for(const auto &mes:  m_sendingMessages) {
            m_queue.push_front(mes);
        }
        m_sendingMessages.clear();

        // Публикуем все сообщения, которые не удалось опубликовать
        // вместе с retain сообщениями
        if(m_queue.size()) {
            if(!m_publishInited) {
                m_publishInited = true;
                QTimer::singleShot(0, this, SLOT(tryToSendMessage()));
            }
        }
        else {
            m_publishInited = false;
        }
        emit connected();

        //QQueue<CMqttMessage> queue{ m_queue };
        //queue.detach();
        //m_queue.clear();

        //for ( const auto & message : queue )
        //{
        //    enqueueMessage( message );
        //}
    }, Qt::AutoConnection);


    connect(m_client, &QMqttClient::messageSent, this,
            &CMqttClientHandler::onMessageSent,
            Qt::AutoConnection);

    if ( m_willMessage.isValid() )
    {
        m_client->setWillQoS( m_willMessage.getQos() );
        m_client->setWillRetain( m_willMessage.getRetain() );
        m_client->setWillTopic( m_willMessage.getTopicName() );
//        m_client->setWillMessage( m_willMessage.getMessage() );
    }

    m_connectTimeoutTimer = new QTimer( this );
    m_connectTimeoutTimer->setSingleShot( true );
    m_connectTimeoutTimer->setInterval( RECONNECT_TIMEOUT );
    connect( m_connectTimeoutTimer, &QTimer::timeout, this,
             &CMqttClientHandler::reconnectSlot, Qt::AutoConnection );

    connect( m_client, &QMqttClient::messageReceived, this,
            [ this ] ( const QByteArray message, const QMqttTopicName topic )
    {
        emit messageReceived( CMqttMessage( topic.name(), message ) );
    });
    reconnectSlot();
}


// Какого-то хрена этот слот не вызывается
// хотя сообщения передаются.
void CMqttClientHandler::onMessageSent(qint32 mesId)
{
    Q_UNUSED(mesId);
    // Сообщение с указанным id передано
    /*if(m_sendingMessages.contains(mesId)) {
        m_sendingMessages.erase(m_sendingMessages.find(mesId));
    }
    if(m_sendingMessages.isEmpty()) {
        txMessageTimeoutTimer->stop();
    }
    else {
        txMessageTimeoutTimer->start(NORM_WAITING_TIMEOUT);
    }*/
}


void CMqttClientHandler::threadStopedSlot()
{
    forceDisconnectSlot();
    m_connectTimeoutTimer->stop();
    txMessageTimeoutTimer->stop();
}


/** ****************************************************************************
 * @fn subscribeSlot
 * @brief слот, получает задание на попытку подписаться на сообщения топика
 * @param QString
 * @param quint8
 * @return bool
 ** ***************************************************************************/
CMqttClientHandler::SubscriptionState CMqttClientHandler::subscribeSlot(
        const QString topicName, const quint8 qos)
{
    SubscriptionState state{ SubscriptionState::Unsubscribed };
    bool connected = false;
    if ( ! m_client )
    {
        printMessage(QString( "Объект MQTT клиента не "
                "проинициализирован! Прежде выполните слот "
                "[threadStartedSlot]" ),
                CLoggerMessage::ELogCriteria::LogCriteria_error,
                m_printLogMessages);
    }
    else
    {
        if ( m_client->state() == QMqttClient::Connected )
        {
            const QMqttSubscription * subscription{ m_client->subscribe( topicName, qos ) };
            if ( subscription == nullptr )
            {

                printMessage(QString( "Не удалось подписаться на топик[%1]!" ).arg( topicName ),
                        CLoggerMessage::ELogCriteria::LogCriteria_error,
                        m_printLogMessages);
            }
            else
            {
                state = static_cast< SubscriptionState >( subscription->state() );
            }
            connected = true;
        }
    }
    if(state == SubscriptionState::Subscribed ||
            state == SubscriptionState::SubscriptionPending) {
        m_subscrubedTopics[topicName] = qos;
    }
    else { // Не удалось подписаться, возможно из-за того, что нет соединения в
        // данный момент. Тем не менее отложим попытку соединения до момена,
        // когда будет восстановлен коннект, поэтому считаем, что состояние
        // у нас pending
        state = SubscriptionState::SubscriptionPending;
        m_subscrubedTopics[topicName] = qos;
        if(connected) { // Соединены и не подписались или не запендили подписку,
                        // ну значит все плохо, делаем реконнект
            forceReconnect();
        }
    }
    emit subscribeResult( state, QPrivateSignal{} );
    return state;
}

/** ****************************************************************************
 * @fn getStateSlot
 * @brief слот, получает задание на возвращение статуса подключения
 * @param none
 * @return ClientState
 ** ***************************************************************************/
void CMqttClientHandler::getStateSlot()
{
    ClientState state{ ClientState::Disconnected };
    if ( ! m_client )
    {
        printMessage( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ),
                CLoggerMessage::ELogCriteria::LogCriteria_error,
                m_printLogMessages);
    }
    else
    {
        state = static_cast< ClientState >( m_client->state() );
    }
    emit getStateResult( state, QPrivateSignal{} );
}

/** ****************************************************************************
 * @fn unsubscribeSlot
 * @brief слот, получает задание на попытку отписаться от сообщений топика
 * @param QString
 * @return none
 ** ***************************************************************************/
bool CMqttClientHandler::unsubscribeSlot(const QString topicName)
{
    bool res{ false };
    if ( ! m_client )
    {
        printMessage( QString( "Объект MQTT клиента не "
                "проинициализирован! Прежде выполните слот "
                "[threadStartedSlot]" ),
                CLoggerMessage::ELogCriteria::LogCriteria_error,
                m_printLogMessages);
    }
    else
    {
        m_client->unsubscribe( topicName );
        res = true;
        if(m_subscrubedTopics.contains(topicName)) {
            m_subscrubedTopics.erase(m_subscrubedTopics.find(topicName));
        }
    }
    emit unsubscribeResult( res, QPrivateSignal{} );
    return res;
}

/** ****************************************************************************
 * @fn setWillMessageSlot
 * @brief слот, получает задание на сохранение will сообщения
 * @param CMqttMessage
 * @return none
 ** ***************************************************************************/
bool CMqttClientHandler::setWillMessageSlot(const CMqttMessage message)
{
    m_willMessage = message;
    emit setWillMessageResult( true, QPrivateSignal{} );
    return true;
}


/** ****************************************************************************
 * @fn enqueueMessage
 * @brief слот, загружает передваемое сообщение в очередь. Метод
 * гарантированно будет вызван в потоке текущего класса, поскольку вызывается
 * с помощью QObject::metaObject::invokeMetod.
 * @param CMqttMessage
 * @return none
 ** ***************************************************************************/
void CMqttClientHandler::enqueueMessage( const CMqttMessage message )
{
    while ( m_queue.size() >= MAXIMUM_QUEUE_SIZE )
    {
        m_queue.dequeue();
    }
    m_queue.enqueue(message);
    if(message.getRetain()) { // retain сообщения сохраняем в map
                              // для повторной публикации после временного
                              //обрыва связи
        m_retainMessages[message.getTopicName()] = message;
    }
    else {
        if(m_retainMessages.contains(message.getTopicName())) { // ранее
            // этот топик был retain.
            m_retainMessages.erase(m_retainMessages.
                    find(message.getTopicName())); // Убираем его
        }
    }
    if(!m_publishInited) {    // Форсируем передачу
        m_publishInited = true;
        QTimer::singleShot(0, this, SLOT(tryToSendMessage()));
    }
}


/** ****************************************************************************
 * @fn tryToSendMessage
 * @brief слот, пытаемся отправлять сообщения, пока очередь не опустошится
 * Слот вызывается только из родного потока с помощью invokeMetod метода
 * @param CMqttMessage
 * @return none
 ** ***************************************************************************/
void CMqttClientHandler::tryToSendMessage()
{
    if(m_queue.size() == 0) {
        m_publishInited = false; // Очередь пустая, закончили передачу
        return;
    }
    if ( static_cast<ClientState>( m_client->state() ) !=
            ClientState::Connected )
    {
        // Отсоединены. Будем пытаться передать еще раз через 100 мс.
        QTimer::singleShot(100, this, SLOT(tryToSendMessage()));

        return;
    }

    const CMqttMessage & msg = m_queue.first();

    CMqttClientHandler::ClientState state = getState();
    QString strState = states.contains(state)? states[state]:
        "Unknown state";

    if (msg.getQos() > 2)
    {
        printMessage(QString("Не удалось опубликовать сообщение "
                             "клиенту %1 к брокеру MQTT IP=%2, port=%3 QOS=%4 из состояния %5 в "
                                                                 "топик %6 по причине неверно "
                             "заданного значения QOS сообщения при его публикации").
                                                      arg(this->clientName()).
                                                      arg(m_settings.getIp()).
                                                      arg(m_settings.getPort()).
                                                      arg(msg.getQos()).
                                                     arg(strState).
                                                     arg(msg.getTopicName() ),
                CLoggerMessage::ELogCriteria::LogCriteria_warning,
                m_printLogMessages);
        m_queue.dequeue();
        if(m_queue.size()) {
            // Передаем следующее сообщение из очереди
            QTimer::singleShot(0, this, SLOT(tryToSendMessage()));
        }
        else {
            m_publishInited = false;
        }
        return;
    }

    qint32 mesId =  m_client->publish( msg.getTopicName(), msg.getMessage(),
                                       msg.getQos(), msg.getRetain() );
    if (mesId < 0 ) // Ошибка публикации сообщения
    {
        //m_queue.append( msg );

        printMessage(QString("Не удалось опубликовать сообщение "
                             "клиенту %1 к брокеру MQTT IP=%2, port=%3 QOS=%4 из состояния %5 в "
                                                                 "топик %6").
                                                      arg(this->clientName()).
                                                      arg(m_settings.getIp()).
                                                      arg(m_settings.getPort()).
                                                      arg(msg.getQos()).
                                                     arg(strState).
                                                     arg(msg.getTopicName() ),
                CLoggerMessage::ELogCriteria::LogCriteria_warning,
                m_printLogMessages);
        QTimer::singleShot(1000, this, SLOT(tryToSendMessage())); // Не удалось
                                              // передать. Будем пытаться
                                              // передать еще раз Через 100 мс.
    }
    else {

        /*// Удалось инициализировать передачу сообщения. Теперь
        // нужно еще дождаться подтверждение приема сообщения брокером,
        // поэтому сохраняем id и передаваемое сообщение до тех пор,
        // пока не получим подтверждения передачи
        m_sendingMessages[mesId] = msg;
        if(!txMessageTimeoutTimer->isActive()) { // Мы еще не ожидаем
                                                // подтверждения передачи
            txMessageTimeoutTimer->start(NORM_WAITING_TIMEOUT);
        }*/

        QString mesInfo = QStringLiteral( "The message (" ) +
                QString::fromLocal8Bit( msg.getMessage() ) +
                QStringLiteral( ") was scheduled to publish to topic " ) +
                msg.getTopicName();
        if(m_notifyPublished) {
            printMessage( QString("Опубликовано сообщение "
            "клиента %1 к брокеру MQTT IP=%2, port=%3  QOS=%4 из состояния %5 в "
                                                "топик %6").
                                     arg(this->clientName()).
                                     arg(m_settings.getIp()).
                                     arg(m_settings.getPort()).
                                    arg(msg.getQos()).
                                    arg(strState).
                                    arg(msg.getTopicName()),
                    CLoggerMessage::ELogCriteria::LogCriteria_info,
                    m_printLogMessages);
        }
        if ( CConsoleLogger::isVerbose())
        {
            printMessage(mesInfo, CLoggerMessage::ELogCriteria::LogCriteria_info,
                    m_printLogMessages);
        }
        m_queue.dequeue();
        if(m_queue.size()) {
            // Передаем следующее сообщение из очереди
            QTimer::singleShot(0, this, SLOT(tryToSendMessage()));
        }
        else {
            m_publishInited = false;
        }
    }
}

void CMqttClientHandler::sendMessageTimeout()
{
    // Собираем все сообщения, которые не были
    // доставлены до адресата в очередь и
    // делаем рестарт
    txMessageTimeoutTimer->stop();
    printMessage( QString( "Не дождались подтверждения "
            "публикации сообщения клиентом %1 "
            "MQTT броккеру IP=%2 PORT=%3. Производим реконнект").
            arg(this->clientName()).
            arg(m_settings.getIp()).
            arg(m_settings.getPort()),
                  CLoggerMessage::ELogCriteria::LogCriteria_error,
            m_printLogMessages);
    forceReconnect();
}


/** ****************************************************************************
 * @fn forceReconnect   [thread safe]
 * @brief слот, вызывает принудительный реконнект
 * @param none
 * @return none
 ** ***************************************************************************/
bool CMqttClientHandler::forceReconnect()
{
    bool res{ false };
    if ( ! QMetaObject::invokeMethod( this,
                                      "reconnectSlot",
                                      Qt::AutoConnection ) )
    {
        printMessage( "Невозможно сформировать invokeMethod!" ,
                CLoggerMessage::ELogCriteria::LogCriteria_error,
                m_printLogMessages);
    }
    else
    {
        res = true;
    }
    return res;
}

/** ****************************************************************************
 * @fn forceDisconnect   [thread safe]
 * @brief слот, вызывает принудительный реконнект
 * @param none
 * @return none
 ** ***************************************************************************/
bool CMqttClientHandler::forceDisconnect()
{
    bool res{ false };
    if ( QThread::currentThread() == thread() )
    {
        res = forceDisconnectSlot();
    }
    else
    {
        QSemaphore sema(0);
        QMetaObject::Connection conn = connect( this, &CMqttClientHandler::forceDisconnectResult, [ &sema, &res ]( const bool state )
        {
            res = state;
            sema.release( 1 );
        });
        if ( ! QMetaObject::invokeMethod( this,
                                          "forceDisconnectSlot",
                                          Qt::AutoConnection ) )
        {
             printMessage("Невозможно сформировать invokeMethod!",
                    CLoggerMessage::ELogCriteria::LogCriteria_error,
                    m_printLogMessages);
        }
        else
        {
            if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
            {
                 printMessage( "Таймаут ожидания исполнения метода",
                        CLoggerMessage::ELogCriteria::LogCriteria_error,
                        m_printLogMessages);
            }
        }
        disconnect( conn );
    }
    return res;
}

/** ****************************************************************************
 * @fn publish      [thread safe]
 * @brief слот, публикует сообщение.
 * @param CMqttMessage
 * @return none
 ** ***************************************************************************/
qint32 CMqttClientHandler::safePublish( const CMqttMessage message )
{
    if ( ! QMetaObject::invokeMethod( this, "enqueueMessage",
            Qt::AutoConnection, Q_ARG( CMqttMessage, message ) ) )
    {
        printMessage( "Невозможно сформировать invokeMethod!",
                CLoggerMessage::ELogCriteria::LogCriteria_error,
                m_printLogMessages);
        return -1;
    }
    return 0;
}


/** ***********************************************************************
 * @fn publish      [thread safe]
 * @brief слот, публикует сообщение. Возвращает id отправленного пакета.
 * Возвращается -1 в случае если:
 *   - некорректный пакет для отправки (см CMqttMessage::isValid)
 *   - не установленн коннект с брокером
 *   - не успешная отправка пакета
 * @param CMqttMessage
 * @return qint32
 ** ***********************************************************************/
qint32 CMqttClientHandler::publish(const CMqttMessage message)
{
    return safePublish(message);
}

/** ***********************************************************************
 * @fn subscribe        [thread safe]
 * @brief слот, подписывается на сообщения топика. Возвращает false в
 * следующих случаях:
 *   - некорректные входные параметры (пустое имя топика или невалидный qos)
 *   - не установленный коннект с брокером
 *   - не удалось подписаться на сообщенимя топика
 * @param QString
 * @param quint8
 * @return bool
 ** ***********************************************************************/
CMqttClientHandler::SubscriptionState CMqttClientHandler::subscribe( const QString topicName, const quint8 qos )
{
    SubscriptionState state{ SubscriptionState::Unsubscribed };
    if ( ! topicName.isEmpty() && qos <= CMqttMessage::MQTT_QOS_MAX )
    {
        if ( QThread::currentThread() == thread() )
        {
            state = subscribeSlot( topicName, qos );
        }
        else
        {
            QSemaphore sema(0);
            QMetaObject::Connection conn = connect( this, &CMqttClientHandler::subscribeResult, [ this, &sema, &state ]( const SubscriptionState stateRes )
            {
                state = stateRes;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "subscribeSlot",
                                              Qt::AutoConnection,
                                              Q_ARG( QString, topicName ),
                                              Q_ARG( quint8, qos ) ) )
            {
                printMessage( "Невозможно сформировать invokeMethod!",
                              CLoggerMessage::ELogCriteria::LogCriteria_error,
                              m_printLogMessages);
            }
            else
            {
                if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
                {
                    printMessage( "Таймаут ожидания исполнения метода",
                            CLoggerMessage::ELogCriteria::LogCriteria_error,
                            m_printLogMessages);
                }
            }
            disconnect( conn );
        }
    }
    return state;
}

/** ***********************************************************************
 * @fn unsubscribe     [thread safe]
 * @brief метод, возвращает состояние сокета
 * @param QString
 * @param quint8
 * @return bool
 ** ***********************************************************************/
bool CMqttClientHandler::unsubscribe(const QString topicName)
{
    bool res{ false };
    if ( ! topicName.isEmpty() )
    {
        if ( QThread::currentThread() == thread() )
        {
            res = unsubscribeSlot( topicName );
        }
        else
        {
            QSemaphore sema(0);
            QMetaObject::Connection conn = connect( this, &CMqttClientHandler::unsubscribeResult, [ this, &sema, &res ]( const bool state )
            {
                res = state;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "unsubscribeSlot",
                                              Qt::AutoConnection,
                                              Q_ARG( QString, topicName ) ) )
            {
                printMessage( "Невозможно сформировать invokeMethod!",
                        CLoggerMessage::ELogCriteria::LogCriteria_error,
                        m_printLogMessages);
            }
            else
            {
                if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
                {
                    printMessage( "Таймаут ожидания исполнения метода",
                            CLoggerMessage::ELogCriteria::LogCriteria_error,
                            m_printLogMessages);
                }
            }
            disconnect( conn );
        }
    }
    return res;
}

/** ***********************************************************************
 * @fn setWillMessage     [thread safe]
 * @brief метод, задает will сообщение (отправляется в случае закрытия клиента)
 * @param CMqttMessage
 * @return bool
 ** ***********************************************************************/
bool CMqttClientHandler::setWillMessage(const CMqttMessage message)
{
    bool res{ false };
    if ( message.isValid() )
    {
        if ( QThread::currentThread() == thread() )
        {
            res = setWillMessageSlot( message );
        }
        else
        {
            QSemaphore sema(0);
            QMetaObject::Connection conn = connect( this, &CMqttClientHandler::setWillMessageResult, [ this, &sema, &res ]( const bool state )
            {
                res = state;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "setWillMessageSlot",
                                              Qt::AutoConnection,
                                              Q_ARG( CMqttMessage, message ) ) )
            {
                printMessage( "Невозможно сформировать invokeMethod!",
                        CLoggerMessage::ELogCriteria::LogCriteria_error,
                        m_printLogMessages);
            }
            else
            {
                if ( ! sema.tryAcquire( 1, FAST_WAITING_TIMEOUT ) )
                {
                    printMessage( "Таймаут ожидания исполнения метода",
                            CLoggerMessage::ELogCriteria::LogCriteria_error,
                            m_printLogMessages);
                }
            }
            disconnect( conn );
        }
    }
    return res;
}

/** ***********************************************************************
 * @fn getState     [thread safe]
 * @brief метод, возвращает состояние сокета
 * @param QString
 * @param quint8
 * @return bool
 ** ***********************************************************************/
CMqttClientHandler::ClientState CMqttClientHandler::getState()
{
    ClientState state{ ClientState::Disconnected };
    if ( QThread::currentThread() == thread() )
    {
        state = static_cast< ClientState >( m_client->state() );
    }
    else
    {
        QSemaphore sema(0);
        QMetaObject::Connection conn = connect( this, &CMqttClientHandler::getStateResult, [ this, &sema, &state ]( const ClientState clientState )
        {
            state = clientState;
            sema.release(1);
        });
        if ( ! QMetaObject::invokeMethod( this,
                                          "getStateSlot",
                                          Qt::AutoConnection ) )
        {
            printMessage( "Невозможно сформировать invokeMethod!",
                    CLoggerMessage::ELogCriteria::LogCriteria_error,
                    m_printLogMessages);
        }
        else
        {
            if ( ! sema.tryAcquire( 1, FAST_WAITING_TIMEOUT ) )
            {
                printMessage( "Таймаут ожидания исполнения метода",
                        CLoggerMessage::ELogCriteria::LogCriteria_error,
                        m_printLogMessages);
            }
        }
        disconnect( conn );
    }
    return state;
}

/** ***********************************************************************
 * @fn getLastError
 * @brief метод, возвращает возможную ошибку после последней операции
 * @param QString
 * @param quint8
 * @return bool
 ** ***********************************************************************/
CMqttClientHandler::ClientError CMqttClientHandler::getError() const
{
    const CMqttClientHandler::ClientError error =
            static_cast< CMqttClientHandler::ClientError >( m_client->error() );
    return error;
}


/** ***********************************************************************
 * @fn createNewConnection     [static]
 * @brief метод, создает новое подключение в указанном потоке
 * (если поток еще не запущен - запускается). Так же происходит ожидание
 * установления подключения (таймаут 10 секунд).
 * После получения указателя на объект желательно проверить состояние подключения
 * к брокеру
 * Может вернуть nullptr, если указатель на поток нулевой.
 * Внимание!
 * - Приняли решение - клиенты Mqtt всегда работают не в главном потоке
 * -Созданные клиенты будут удаляться автоматически при остановке потока.
 * Не следует их удалять самостоятельно, посколько это может привести
 * к крешу.
 *
 * @param QThread *
 * @param CMqttConnectionSettings
 * @param CMqttMessage
 * @param QObject *
 * @return QMqttClient *
 ** ***********************************************************************/
CMqttClientHandler *CMqttClientHandler::createNewConnection(QThread * thread,
        const CMqttConnectionSettings &settings,
        const CMqttMessage &willMessage,
        QObject * parent,
        const QString &clientName,
        bool printLogMessages)
{
    Q_UNUSED(parent);
    Q_UNUSED(willMessage);
    CMqttClientHandler * mqttClient = nullptr;
    if ( thread == nullptr )
    {
        printMessage( "Некорректный входной указатель на поток",
                CLoggerMessage::ELogCriteria::LogCriteria_error,
                printLogMessages);
        return mqttClient;
    }

    mqttClient = new CMqttClientHandler( settings, nullptr, clientName);
    mqttClient->setPrintLogMessages(printLogMessages);
    if ( QThread::currentThread() != thread )
    {
        mqttClient->moveToThread( thread );
    }
    QObject::connect(thread, &QThread::finished, mqttClient, &QObject::deleteLater);
    if ( thread->isRunning() )
    {
        if ( ! QMetaObject::invokeMethod( mqttClient,
                                          "threadStartedSlot",
                                          Qt::AutoConnection ) )
        {
            printMessage( "Невозможно сформировать invokeMethod!",
                    CLoggerMessage::ELogCriteria::LogCriteria_error,
                    printLogMessages);
        }
    }
    else
    {
        printMessage( "DEBUG: Поток не запущен. Запускаем поток и инициируем подключение клиента",
                CLoggerMessage::ELogCriteria::LogCriteria_info,
                printLogMessages);
        QObject::connect( thread, &QThread::started, mqttClient, &CMqttClientHandler::threadStartedSlot );
        thread->start();

        QSemaphore sema(0);
        auto connLambda = [ &sema ]()
        {
            sema.release(1);
        };
        auto conn1 = QObject::connect( mqttClient, &CMqttClientHandler::connected, connLambda );
        auto conn2 = QObject::connect( mqttClient, &CMqttClientHandler::disconnected, connLambda );
        if ( ! sema.tryAcquire( 1, RECONNECT_TIMEOUT ) )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
        }
        disconnect( conn1 );
        disconnect( conn2 );
    }
    return mqttClient;
//    if ( QThread::currentThread() != thread ) mqttClient->moveToThread( thread );
//    QEventLoop loop{};
//    QTimer tm;
//    QObject::connect( mqttClient, &CMqttClientHandler::connected, & loop, &QEventLoop::quit );
//    QObject::connect( mqttClient, &CMqttClientHandler::disconnected, & loop, &QEventLoop::quit );
//    QObject::connect(thread, &QThread::finished, mqttClient, &QObject::deleteLater);
//    QObject::connect(&tm, &QTimer::timeout, &loop, &QEventLoop::quit, Qt::AutoConnection);
//    if ( thread->isRunning() )
//    {
//        if ( ! QMetaObject::invokeMethod( mqttClient,
//                                          "threadStartedSlot",
//                                          Qt::AutoConnection ) )
//        {
//            printMessage( "Невозможно сформировать invokeMethod!",
//                    CLoggerMessage::ELogCriteria::LogCriteria_error,
//                    printLogMessages);
//        }
//        else
//        {
//            //      printMessage( "DEBUG: Поток уже запущен, инициируем подключение клиента",
//            // CLoggerMessage::ELogCriteria::LogCriteria_info );
//        }
//    }
//    else
//    {
//        printMessage( "DEBUG: Поток не запущен. Запускаем поток и инициируем подключение клиента",
//                CLoggerMessage::ELogCriteria::LogCriteria_info,
//                printLogMessages);
//        QObject::connect( thread, &QThread::started, mqttClient, &CMqttClientHandler::threadStartedSlot );
//        thread->start();
//    }
//    tm.start(RECONNECT_TIMEOUT);
//    loop.exec(QEventLoop::EventLoopExec);
//    return mqttClient;
}


/** ****************************************************************************
 * @fn createNewConnection     [static]
 * @brief метод, перегруженный метод createNewConnection( QThread * thread,
 *       const CMqttConnectionSettings &settings,
 *       const CMqttMessage willMessage,
 *       QObject * parent )
 * Без параметров настроек. Настройки соединения будут автоматически
 * считаны из файла настроек приложения
 * @return QMqttClient *
 ** ***************************************************************************/
CMqttClientHandler *CMqttClientHandler::createNewConnection( QThread * thread,
        const CMqttMessage &willMessage,
        QObject * parent,
        const QString &clientName,
        bool printLogMessages)
{
    return createNewConnection(thread, CMqttConnectionSettings(),
            willMessage, parent, clientName, printLogMessages);
}
