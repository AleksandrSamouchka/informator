#include "CMqttClientHandlerNew.h"
#include "CConsoleLogger.h"
#include <QtMqtt/qmqttclient.h>
#include <QtCore/QTimer>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>
#include <QtCore/QEventLoop>

namespace
{
    const int RECONNECT_TIMEOUT{ 5 * 1000 };
    const int FAST_WAITING_TIMEOUT{ 100 };
    const int NORM_WAITING_TIMEOUT{ 5000 };
    const int MAXIMUM_QUEUE_SIZE{ 10000 };
}

/** ***********************************************************************
 * @fn CMqttClientHandlerNew  [конструктор]
 ** ***********************************************************************/
CMqttClientHandler::CMqttClientHandler(QObject * parent) :
    QObject( parent )
{
    static auto id1 = qRegisterMetaType<ClientState>("ClientState");
    static auto id2 = qRegisterMetaType<SubscriptionState>("SubscriptionState");
    static auto id3 = qRegisterMetaType<CMqttMessage>("CMqttMessage");
    Q_UNUSED( id1 );
    Q_UNUSED( id2 );
    Q_UNUSED( id3 );
    m_queue.reserve(MAXIMUM_QUEUE_SIZE);
    connect( this, & CMqttClientHandler::destroyed, this, &CMqttClientHandler::threadStopedSlot );
}

CMqttClientHandler::CMqttClientHandler(const CMqttConnectionSettings settings, QObject * parent) :
    CMqttClientHandler( parent )
{
    m_settings = settings;
}

/** ***********************************************************************
 * @fn reconnectTimerSlot
 * @brief слот, выполняет переподключение к брокеру
 * @param none
 * @return none
 ** ***********************************************************************/
const QMqttClient *CMqttClientHandler::getMqttClient()
{
    return m_client;
}

void CMqttClientHandler::reconnectSlot()
{
    if ( ! m_client )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не "
                "проинициализирован! Прежде выполните слот "
                                              "[threadStartedSlot]" ) );
        return;
    }
    const ClientState state{ static_cast< ClientState >( m_client->state() ) };
    if ( state == ClientState::Disconnected )
    {
        if ( ! m_settings.isValid() )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( QString("Некорректные настройки подключения к брокеру MQTT [%1] - проверьте настройки!").arg( m_settings.getLastError() ) );
        }
        else
        {
            m_client->setHostname( m_settings.getIp() );
            m_client->setPort( m_settings.getPort() );
            m_connectTimeoutTimer->start();
            m_client->connectToHost();
        }
    }
    else if ( state == ClientState::Connecting )
    {
        if ( QAbstractSocket * socket = qobject_cast< QAbstractSocket * >( m_client->transport() ) )
        {
            socket->waitForConnected( 1 );
            socket->blockSignals( true );
            m_client->disconnectFromHost();
            socket->blockSignals( false );
            //m_client->setState( QMqttClient::Disconnected );
            if ( ! m_settings.isValid() )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( QString("Некорректные настройки подключения к брокеру MQTT [%1] - проверьте настройки!").arg( m_settings.getLastError() ) );
            }
            else
            {
                m_client->setHostname( m_settings.getIp() );
                m_client->setPort( m_settings.getPort() );
                m_connectTimeoutTimer->start();
                m_client->connectToHost();
            }
        }
        else
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Некорректный транспортный QAbstractSocket!" );
        }
    }
    else
    {
        m_client->disconnectFromHost();
    }

}

/** ***********************************************************************
 * @fn forceDisconnectSlot
 * @brief слот, получает задание на отключение от брокера
 * @param none
 * @return none
 ** ***********************************************************************/
bool CMqttClientHandler::forceDisconnectSlot()
{
    bool res{ false };
    if ( ! m_client )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
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
                PRINT_CONSOLE_MESSAGE_ERROR( "Некорректный транспортный QAbstractSocket!" );
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

/** ***********************************************************************
 * @fn threadStart
 * @brief слот, вызывается после переноса объекта в нужный поток и его запуска.
 * Если объект не будет переноситься в отдельный поток - данный слот запускать
 * сразу
 * @param none
 * @return none
 ** ***********************************************************************/
void CMqttClientHandler::threadStartedSlot()
{
    m_client = new QMqttClient(this);
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
        m_connectTimeoutTimer->stop();
        emit connected();
        if(m_queue.size()) {
            if(!publishInited) {
                publishInited = true;
                QTimer::singleShot(0, this, SLOT(tryToSendMessage()));
            }
        }
        else {
            publishInited = false;
        }
        // Восстанавливаем подписки на топики
        for(auto it = subscrubedTopics.begin(); it!=subscrubedTopics.end(); it++) {
            subscribeSlot(it.key(), it.value());
        }

        //QQueue<CMqttMessage> queue{ m_queue };
        //queue.detach();
        //m_queue.clear();

        //for ( const auto & message : queue )
        //{
        //    enqueueMessage( message );
        //}
    }, Qt::AutoConnection);


    connect(m_client, &QMqttClient::messageSent, this, [this](qint32) {
        // Сообщение с указанным id передано

    }, Qt::AutoConnection);


    if ( m_willMessage.isValid() )
    {
        m_client->setWillQoS( m_willMessage.getQos() );
        m_client->setWillRetain( m_willMessage.getRetain() );
        m_client->setWillTopic( m_willMessage.getTopicName() );
        m_client->setWillMessage( m_willMessage.getMessage() );
    }

    m_connectTimeoutTimer = new QTimer( this );
    m_connectTimeoutTimer->setSingleShot( true );
    m_connectTimeoutTimer->setInterval( RECONNECT_TIMEOUT );
    connect( m_connectTimeoutTimer, &QTimer::timeout, this, &CMqttClientHandler::reconnectSlot, Qt::AutoConnection );

    connect( m_client, &QMqttClient::messageReceived, this, [ this ] ( const QByteArray message, const QMqttTopicName topic )
    {
        emit messageReceived( CMqttMessage( topic.name(), message ) );
    });
    reconnectSlot();
}

void CMqttClientHandler::threadStopedSlot()
{
    forceDisconnectSlot();
    m_connectTimeoutTimer->stop();
}

/** ***********************************************************************
 * @fn publishSlot
 * @brief слот, получает задание на публикацию сообщения
 * @param CMqttMessage
 * @return none
 ** ***********************************************************************/
qint32 CMqttClientHandler::publishSlot(const CMqttMessage message)
{
    qint32 id{ -1 };
    if ( ! m_client )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
    }
    else
    {
        if ( m_client->state() == QMqttClient::Connected )
        {
            id = m_client->publish( message.getTopicName(), message.getMessage(), message.getQos(), message.getRetain() );
        }
    }
    emit publishResult( id, QPrivateSignal{} );
    return id;
}

/** ***********************************************************************
 * @fn subscribeSlot
 * @brief слот, получает задание на попытку подписаться на сообщения топика
 * @param QString
 * @param quint8
 * @return bool
 ** ***********************************************************************/
CMqttClientHandler::SubscriptionState CMqttClientHandler::subscribeSlot(const QString topicName, const quint8 qos)
{
    SubscriptionState state{ SubscriptionState::Unsubscribed };
    if ( ! m_client )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
    }
    else
    {
        if ( m_client->state() == QMqttClient::Connected )
        {
            const QMqttSubscription * subscription{ m_client->subscribe( topicName, qos ) };
            if ( subscription == nullptr )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( QString( "Не удалось подписаться на топик[%1]!" ).arg( topicName ) );
            }
            else
            {
                state = static_cast< SubscriptionState >( subscription->state() );
            }
        }
    }
    if(state == SubscriptionState::Subscribed ||
            state == SubscriptionState::SubscriptionPending) {
        subscrubedTopics[topicName] = qos;
    }

    emit subscribeResult( state, QPrivateSignal{} );
    return state;
}

/** ***********************************************************************
 * @fn getStateSlot
 * @brief слот, получает задание на возвращение статуса подключения
 * @param none
 * @return ClientState
 ** ***********************************************************************/
void CMqttClientHandler::getStateSlot()
{
    ClientState state{ ClientState::Disconnected };
    if ( ! m_client )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
    }
    else
    {
        state = static_cast< ClientState >( m_client->state() );
    }
    emit getStateResult( state, QPrivateSignal{} );
}

/** ***********************************************************************
 * @fn unsubscribeSlot
 * @brief слот, получает задание на попытку отписаться от сообщений топика
 * @param QString
 * @return none
 ** ***********************************************************************/
bool CMqttClientHandler::unsubscribeSlot(const QString topicName)
{
    bool res{ false };
    if ( ! m_client )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
    }
    else
    {
        m_client->unsubscribe( topicName );
        res = true;
        if(subscrubedTopics.contains(topicName)) {
            subscrubedTopics.erase(subscrubedTopics.find(topicName));
        }
    }
    emit unsubscribeResult( res, QPrivateSignal{} );
    return res;
}

/** ***********************************************************************
 * @fn setWillMessageSlot
 * @brief слот, получает задание на сохранение will сообщения
 * @param CMqttMessage
 * @return none
 ** ***********************************************************************/
bool CMqttClientHandler::setWillMessageSlot(const CMqttMessage message)
{
    m_willMessage = message;
    emit setWillMessageResult( true, QPrivateSignal{} );
    return true;
}


/** ***********************************************************************
 * @fn enqueueMessage
 * @brief слот, загружает передваемое сообщение в очередь. Метод
 * гарантированно будет вызван в потоке текущего класса, поскольку вызывается
 * с помощью QObject::metaObject::invokeMetod.
 * @param CMqttMessage
 * @return none
 ** ***********************************************************************/
void CMqttClientHandler::enqueueMessage( const CMqttMessage message )
{
    while ( m_queue.size() >= MAXIMUM_QUEUE_SIZE )
    {
        m_queue.dequeue();
    }
    m_queue.enqueue(message);

    if(!publishInited) {    // Форсируем передачу
        publishInited = true;
        QTimer::singleShot(0, this, SLOT(tryToSendMessage()));
    }
}


/** ***********************************************************************
 * @fn tryToSendMessage
 * @brief слот, пытаемся отправлять сообщения, пока очередь не опустошится
 * @param CMqttMessage
 * @return none
 ** ***********************************************************************/
void CMqttClientHandler::tryToSendMessage()
{
    if(m_queue.size() == 0) {
        publishInited = false; // Очередь пустая, закончили передачу
        return;
    }
    if ( static_cast<ClientState>( m_client->state() ) != ClientState::Connected )
    {
        QTimer::singleShot(100, this, SLOT(tryToSendMessage())); // Отсоединены
                                                // Будем пытаться передать еще раз
                                                // Через 100 мс.
        return;
    }

    CMqttMessage & msg = m_queue.first();
    if ( m_client->publish( msg.getTopicName(), msg.getMessage(), msg.getQos(), msg.getRetain() ) < 0 )
    {
        //m_queue.append( msg );
        PRINT_CONSOLE_MESSAGE_WARN( QStringLiteral( "Can't publish message, error: " ) +
                                    QString::number( m_client->error() ) );
        QTimer::singleShot(100, this, SLOT(tryToSendMessage())); // Не удалось передать
                                                // Будем пытаться передать еще раз
                                                // Через 100 мс.
    }
    else {
        if ( CConsoleLogger::isVerbose() )
        {
            PRINT_CONSOLE_MESSAGE_INFO( QStringLiteral( "The message (" ) +
                                    QString::fromLocal8Bit( msg.getMessage() ) +
                                    QStringLiteral( ") was published to topic " ) +
                                    msg.getTopicName() );
        }
        m_queue.dequeue();
        if(m_queue.size()) {
            QTimer::singleShot(0, this, SLOT(tryToSendMessage())); // Передаем следующее
                                                                   // сообщение из очереди
        }
        else publishInited = false;
    }
}



/** ***********************************************************************
 * @fn forceReconnect   [thread safe]
 * @brief слот, вызывает принудительный реконнект
 * @param none
 * @return none
 ** ***********************************************************************/
bool CMqttClientHandler::forceReconnect()
{
    bool res{ false };
    if ( ! QMetaObject::invokeMethod( this,
                                      "reconnectSlot",
                                      Qt::AutoConnection ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
    }
    else
    {
        res = true;
    }
    return res;
}

/** ***********************************************************************
 * @fn forceDisconnect   [thread safe]
 * @brief слот, вызывает принудительный реконнект
 * @param none
 * @return none
 ** ***********************************************************************/
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
        QMetaObject::Connection conn = connect( this, &CMqttClientHandler::forceDisconnectResult, [ this, &sema, &res ]( const bool state )
        {
            res = state;
            sema.release( 1 );
        });
        if ( ! QMetaObject::invokeMethod( this,
                                          "forceDisconnectSlot",
                                          Qt::AutoConnection ) )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
        }
        else
        {
            if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
            }
        }
        disconnect( conn );
    }
    return res;
}

/** ***********************************************************************
 * @fn publish      [thread safe]
 * @brief слот, публикует сообщение.
 * @param CMqttMessage
 * @return none
 ** ***********************************************************************/
qint32 CMqttClientHandler::safePublish( const CMqttMessage message )
{
    /*if ( QThread::currentThread() == thread() )
    {
        enqueueMessage( message );
    }
    else if ( ! QMetaObject::invokeMethod( this, "enqueueMessage",
            Qt::AutoConnection, Q_ARG( CMqttMessage, message ) ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );

    }*/
    if ( ! QMetaObject::invokeMethod( this, "enqueueMessage",
                                      Qt::AutoConnection, Q_ARG( CMqttMessage, message ) ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
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
    /*qint32 id{ -1 };
    if (!message.isValid() )
    {
         return id;
    }
    if ( QThread::currentThread() == thread() )
    {
        id = publishSlot( message );
        return id;
    }
    QSemaphore sema(0);
    QMetaObject::Connection conn = connect( this, &CMqttClientHandlerNew::publishResult, [ this, &sema, &id ]( const qint32 messageId )
    {
        id = messageId;
        sema.release( 1 );
    });
    if ( ! QMetaObject::invokeMethod( this,
                                      "publishSlot",
                                      Qt::AutoConnection,
                                      Q_ARG( CMqttMessage, message ) ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
    }
    else
    {
        if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
        }
    }
    disconnect( conn );*/
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
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            else
            {
                if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
                {
                    PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
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
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            else
            {
                if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
                {
                    PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
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
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            else
            {
                if ( ! sema.tryAcquire( 1, FAST_WAITING_TIMEOUT ) )
                {
                    PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
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
            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
        }
        else
        {
            if ( ! sema.tryAcquire( 1, FAST_WAITING_TIMEOUT ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
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
 * Приняли решение - клиенты Mqtt всегда работают не в главном потоке
 * @param QThread *
 * @param CMqttMessage
 * @param QObject *
 * @return QMqttClient *
 ** ***********************************************************************/
CMqttClientHandler *CMqttClientHandler::createNewConnection( QThread * thread,
                                                             const CMqttMessage willMessage,
                                                             QObject * parent )
{
    CMqttClientHandler * mqttClient = nullptr;
    if ( thread == nullptr )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Некорректный входной указатель на поток" );
    }
    else
    {
        if ( QThread::currentThread() == thread )
        {
            mqttClient = new CMqttClientHandler( parent );
            mqttClient->setWillMessage( willMessage );
            QEventLoop loop{};
            QObject::connect( mqttClient, &CMqttClientHandler::connected, & loop, &QEventLoop::quit );
            QObject::connect( mqttClient, &CMqttClientHandler::disconnected, & loop, &QEventLoop::quit );
            if ( ! QMetaObject::invokeMethod( mqttClient,
                                              "threadStartedSlot",
                                              Qt::AutoConnection ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            loop.exec(QEventLoop::EventLoopExec);
        }
        else
        {
            mqttClient = new CMqttClientHandler();
            mqttClient->setWillMessage( willMessage );
            mqttClient->moveToThread( thread );
            QEventLoop loop{};
            QObject::connect( mqttClient, &CMqttClientHandler::connected, & loop, &QEventLoop::quit );
            QObject::connect( mqttClient, &CMqttClientHandler::disconnected, & loop, &QEventLoop::quit );
            if ( thread->isRunning() )
            {
                if ( ! QMetaObject::invokeMethod( mqttClient,
                                                  "threadStartedSlot",
                                                  Qt::AutoConnection ) )
                {
                    PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
                }
                else
                {
//                    PRINT_CONSOLE_MESSAGE_INFO( "DEBUG: Поток уже запущен, инициируем подключение клиента" );
                }
            }
            else
            {
                PRINT_CONSOLE_MESSAGE_INFO( "DEBUG: Поток не запущен. Запускаем поток и инициируем подключение клиента" );
                QObject::connect( thread, &QThread::started, mqttClient, &CMqttClientHandler::threadStartedSlot );
                thread->start();
            }
            loop.exec(QEventLoop::EventLoopExec);
        }
//        PRINT_CONSOLE_MESSAGE_INFO( "DEBUG: Mqtt клиент создан. Проверьте статус подключения" );
    }
    return mqttClient;
}

/** ***********************************************************************
 * @fn createNewConnection     [static]
 * @brief метод, создает новое подключение в указанном потоке
 * (если поток еще не запущен - запускается). Так же происходит ожидание
 * установления подключения (таймаут 10 секунд).
 * После получения указателя на объект желательно проверить состояние подключения
 * к брокеру
 * Может вернуть nullptr, если указатель на поток нулевой.
 * Приняли решение - клиенты Mqtt всегда работают не в главном потоке
 * @param QThread *
 * @param CMqttConnectionSettings
 * @param CMqttMessage
 * @param QObject *
 * @return QMqttClient *
 ** ***********************************************************************/
CMqttClientHandler *CMqttClientHandler::createNewConnectionWithSettings( QThread * thread,
                                                                         const CMqttConnectionSettings settings,
                                                                         const CMqttMessage willMessage,
                                                                         QObject * parent )
{
    CMqttClientHandler * mqttClient = nullptr;
    if ( thread == nullptr )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Некорректный входной указатель на поток" );
    }
    else
    {
        if ( QThread::currentThread() == thread )
        {
            mqttClient = new CMqttClientHandler( settings, parent );
            mqttClient->setWillMessage( willMessage );
            QEventLoop loop{};
            QObject::connect( mqttClient, &CMqttClientHandler::connected, & loop, &QEventLoop::quit );
            QObject::connect( mqttClient, &CMqttClientHandler::disconnected, & loop, &QEventLoop::quit );
            if ( ! QMetaObject::invokeMethod( mqttClient,
                                              "threadStartedSlot",
                                              Qt::AutoConnection ) )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
            }
            loop.exec(QEventLoop::EventLoopExec);
        }
        else
        {
            mqttClient = new CMqttClientHandler( settings );
            mqttClient->setWillMessage( willMessage );
            mqttClient->moveToThread( thread );
            QEventLoop loop{};
            QObject::connect( mqttClient, &CMqttClientHandler::connected, & loop, &QEventLoop::quit );
            QObject::connect( mqttClient, &CMqttClientHandler::disconnected, & loop, &QEventLoop::quit );
            if ( thread->isRunning() )
            {
                if ( ! QMetaObject::invokeMethod( mqttClient,
                                                  "threadStartedSlot",
                                                  Qt::AutoConnection ) )
                {
                    PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
                }
                else
                {
//                    PRINT_CONSOLE_MESSAGE_INFO( "DEBUG: Поток уже запущен, инициируем подключение клиента" );
                }
            }
            else
            {
                PRINT_CONSOLE_MESSAGE_INFO( "DEBUG: Поток не запущен. Запускаем поток и инициируем подключение клиента" );
                QObject::connect( thread, &QThread::started, mqttClient, &CMqttClientHandler::threadStartedSlot );
                thread->start();
            }
            loop.exec(QEventLoop::EventLoopExec);
        }
//        PRINT_CONSOLE_MESSAGE_INFO( "DEBUG: Mqtt клиент создан. Проверьте статус подключения" );
    }
    return mqttClient;
}
