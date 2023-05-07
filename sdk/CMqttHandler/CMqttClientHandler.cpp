#include <QtMqtt/qmqttclient.h>
#include <QtCore/QTimer>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>
#include <QtCore/QPointer>
#include <QtCore/QScopedPointer>
#include <QtNetwork/QHostAddress>

#include "CMqttClientHandler.h"
#include "CConsoleLogger/CConsoleLogger.h"


namespace
{
    bool passReconnect{ false };

    constexpr int RECONNECT_TIMEOUT{ 10 * 1000 };

    const int FAST_WAITING_TIMEOUT{ 100 };
    const int NORM_WAITING_TIMEOUT{ 5000 };
    const int MAXIMUM_QUEUE_SIZE{ 10000 };

    QPointer < CMqttClientHandler > localClient = nullptr;
    QPointer < QThread > localClientThread = nullptr;
    QMutex instanceMutex;
    QMutex initHandlerMutex;
}

static void printInitError()
{
    PRINT_CONSOLE_MESSAGE_ERROR( "Объект MQTT клиента не проинициализирован! "
                                 "Прежде выполните слот [threadStartedSlot]"  );
}

/** ***********************************************************************
 * @fn CMqttClientHandler  [конструктор]
 ** ***********************************************************************/
/*CMqttClientHandler::CMqttClientHandler(QObject * parent) :
    QObject( parent )
{
    init("", 0);
}
*/

/*CMqttClientHandler::CMqttClientHandler(const CMqttConnectionSettings & settings, QObject * parent) :
    CMqttClientHandler( parent )
{
//    m_settings = settings;
    m_host = settings.getIp();
    m_port = settings.getPort();
}
*/
CMqttClientHandler::CMqttClientHandler(const QString &host, quint16 port, const QString &clientId, QObject * parent):
    QObject( parent )
{
    init(host, port, clientId, QString(), QString());
}

CMqttClientHandler::CMqttClientHandler(const QString &host, quint16 port, const QString &clientId, const QString &user, const QString &password, QObject * parent) :
    QObject( parent )
{
//    m_settings = settings;
//    m_host = host;
//    m_port = port;
    init(host, port, clientId, user, password);
}

void CMqttClientHandler::init(const QString &host, quint16 port, const QString &clientId, const QString &user, const QString &password)
{
    m_host = host;
    m_port = port;
    m_clientId = clientId;
    m_user = user;
    m_password = password;

    if(m_host.isEmpty())
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Host name is empty." );
    }

    if(m_port == 0)
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Invalid port value" );
    }

    if(m_user.isEmpty()){
        PRINT_CONSOLE_MESSAGE_WARN( "Username is empty." );
    }
    if(m_password.isEmpty()){
        PRINT_CONSOLE_MESSAGE_WARN( "Password is empty." );
    }

    /*static auto id1 = */qRegisterMetaType<ClientState>("ClientState");
    /*static auto id2 = */qRegisterMetaType<SubscriptionState>("SubscriptionState");
    /*static auto id3 = */qRegisterMetaType<CMqttMessage>("CMqttMessage");
//    Q_UNUSED( id1 )
//    Q_UNUSED( id2 )
//    Q_UNUSED( id3 )

//    connect( this, & CMqttClientHandler::destroyed, this, &CMqttClientHandler::threadStopedSlot );
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
        printInitError();
    }
    else
    {
        const ClientState state{ static_cast< ClientState >( m_client->state() ) };
        if ( state == ClientState::Disconnected )
        {
/*            if ( ! m_settings.isValid() )
            {
                PRINT_CONSOLE_MESSAGE_ERROR( QString("Некорректные настройки подключения к брокеру MQTT [%1] - проверьте настройки!").arg( m_settings.getLastError() ) );
            }
            else
            {*/
                m_client->setHostname( m_host );
                m_client->setPort( m_port );

                if(!m_user.isEmpty() && !m_password.isEmpty()){
                    m_client->setUsername(m_user);
                    m_client->setPassword(m_password);
                }

                m_client->connectToHost();
                m_connectTimeoutTimer->start();
//            }
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
/*                if ( ! m_settings.isValid() )
                {
                    PRINT_CONSOLE_MESSAGE_ERROR( QString("Некорректные настройки подключения к брокеру MQTT [%1] - проверьте настройки!").arg( m_settings.getLastError() ) );
                }
                else
                {*/
                    m_client->setHostname( m_host );
                    m_client->setPort( m_port );

                    if(!m_user.isEmpty() && !m_password.isEmpty()){
                        m_client->setUsername(m_user);
                        m_client->setPassword(m_password);
                    }

                    m_client->connectToHost();
                    m_connectTimeoutTimer->start();
//                }
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
//        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
        printInitError();
    }
    else
    {
        passReconnect = true;
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
    m_client->setClientId(m_clientId);
    connect( m_client, &QMqttClient::disconnected, this, [ this ]()
    {
        if ( ! passReconnect )
        {
            reconnectSlot();
        }
        else
        {
            passReconnect = false;
        }
        emit disconnected();
    }, Qt::UniqueConnection );
    connect( m_client, &QMqttClient::connected, this, [ this ]()
    {
        m_connectTimeoutTimer->stop();
        emit connected();

        QList<CMqttMessage> queue{ m_queue };
        queue.detach();
        m_queue.clear();

        for ( const auto & message : queue )
        {
            enqueueMessage( message );
        }
    }, Qt::UniqueConnection );
    if ( m_willMessage.isValid() )
    {
        m_client->setWillQoS( m_willMessage.getQos() );
        m_client->setWillRetain( m_willMessage.getRetain() );
        m_client->setWillTopic( m_willMessage.getTopicName() );
        m_client->setWillMessage( m_willMessage.getMessage() );
    }

    if ( m_connectTimeoutTimer == nullptr )
    {
        m_connectTimeoutTimer = new QTimer( this );
        m_connectTimeoutTimer->setSingleShot( true );
        m_connectTimeoutTimer->setInterval( RECONNECT_TIMEOUT );
        connect( m_connectTimeoutTimer, &QTimer::timeout, this, &CMqttClientHandler::reconnectSlot, Qt::QueuedConnection );
    }
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
//    thread()->deleteLater();
}

/** ***********************************************************************
 * @fn publishSlot
 * @brief слот, получает задание на публикацию сообщения
 * @param CMqttMessage
 * @return none
 ** ***********************************************************************/
qint32 CMqttClientHandler::publishSlot(const CMqttMessage &message)
{
    qint32 id{ -1 };
    if ( ! m_client )
    {
//        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
        printInitError();
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
CMqttClientHandler::SubscriptionState CMqttClientHandler::subscribeSlot(const QString &topicName, quint8 qos)
{
    SubscriptionState state{ SubscriptionState::Unsubscribed };
    if ( ! m_client )
    {
//        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
        printInitError();
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
//        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
        printInitError();
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
bool CMqttClientHandler::unsubscribeSlot(const QString &topicName)
{
    bool res{ false };
    if ( ! m_client )
    {
//        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
        printInitError();
    }
    else
    {
        m_client->unsubscribe( topicName );
        res = true;
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
bool CMqttClientHandler::setWillMessageSlot(const CMqttMessage &message)
{
    m_willMessage = message;
    emit setWillMessageResult( true, QPrivateSignal{} );
    return true;
}

void CMqttClientHandler::enqueueMessage( const CMqttMessage &message )
{
    while ( m_queue.size() > MAXIMUM_QUEUE_SIZE )
    {
        m_queue.pop_front();
    }
    m_queue.append( message );

    if ( ! m_client )
    {
//        PRINT_CONSOLE_MESSAGE_ERROR( QString( "Объект MQTT клиента не проинициализирован! Прежде выполните слот [threadStartedSlot]" ) );
        printInitError();
    }
    else
    {
        if ( static_cast<ClientState>( m_client->state() ) == ClientState::Connected )
        {
            QList<CMqttMessage> queue{ m_queue };
            queue.detach();
            m_queue.clear();

            while ( ! queue.isEmpty() )
            {
                CMqttMessage & msg = queue.first();
                if ( m_client->publish( msg.getTopicName(), msg.getMessage(), msg.getQos(), msg.getRetain() ) < 0 )
                {
                    m_queue.append( msg );
                    PRINT_CONSOLE_MESSAGE_WARN( QStringLiteral( "Can't publish message, error: " ) +
                                                QString::number( m_client->error() ) );
                }
                else if ( CConsoleLogger::isVerbose() )
                {
                    PRINT_CONSOLE_MESSAGE_INFO( QStringLiteral( "The message (" ) +
                                                QString::fromLocal8Bit( msg.getMessage() ) +
                                                QStringLiteral( ") was published to topic " ) +
                                                msg.getTopicName() );
                }
                queue.pop_front();
            }
        }
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
                                      Qt::QueuedConnection ) )
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
        QMetaObject::Connection conn = connect( this, &CMqttClientHandler::forceDisconnectResult, [ &sema, &res ]( const bool state )
        {
            res = state;
            sema.release( 1 );
        });
        if ( ! QMetaObject::invokeMethod( this,
                                          "forceDisconnectSlot",
                                          Qt::QueuedConnection ) )
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
bool CMqttClientHandler::safePublish( const CMqttMessage &message )
{
    if ( QThread::currentThread() == thread() )
    {
        enqueueMessage( message );
    }
    else if ( ! QMetaObject::invokeMethod( this,
                                           "enqueueMessage",
                                           Qt::QueuedConnection,
                                           Q_ARG( CMqttMessage, message ) ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );

    }
    return true;
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
/*
    WORKAROUND FOR INFORMATOR!!

*/
CMqttClientHandler::SubscriptionState CMqttClientHandler::subscribe( CMqttTopic &topic/*const QString &topicName, quint8 qos*/ )
{
    const auto state{ subscribe( topic.topicName() ) };
    const auto result{ state == CMqttClientHandler::SubscriptionState::Subscribed ||
                       state == CMqttClientHandler::SubscriptionState::SubscriptionPending };


//    return subscribe(topic.getName(), qos);
                    if(result)
                    {
                        connect(m_client, &QMqttClient::messageReceived, [&](/*const CMqttMessage &message*/const QByteArray &message, const QMqttTopicName &topicName) {
                                if(topicName == topic.topicName())
                                {
                                    if(topic.initFromData(message))
                                    {
                                        emit topic.messageReceived();
                                    }
                                }
                                });

                    }

    return state;
}


CMqttClientHandler::SubscriptionState CMqttClientHandler::subscribe( const QString &topicName, quint8 qos )
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
            QMetaObject::Connection conn = connect( this, &CMqttClientHandler::subscribeResult, [ &sema, &state ]( const SubscriptionState stateRes )
            {
                state = stateRes;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "subscribeSlot",
                                              Qt::QueuedConnection,
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

CMqttClientHandler::SubscriptionState CMqttClientHandler::subscribe( CTopicValueEntry::ETopicsNamesIndex topicIndex, quint8 qos)
{
    QString topicName = CTopicValueEntry::getTopicName( topicIndex );
    return subscribe(topicName, qos);
}


/** ***********************************************************************
 * @fn unsubscribe     [thread safe]
 * @brief метод, возвращает состояние сокета
 * @param QString
 * @param quint8
 * @return bool
 ** ***********************************************************************/
bool CMqttClientHandler::unsubscribe(const CMqttTopic &topic)
{
    return unsubscribe(topic.topicName());
}

bool CMqttClientHandler::unsubscribe(const QString &topicName)
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
            QMetaObject::Connection conn = connect( this, &CMqttClientHandler::unsubscribeResult, [ &sema, &res ]( const bool state )
            {
                res = state;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "unsubscribeSlot",
                                              Qt::QueuedConnection,
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

bool CMqttClientHandler::unsubscribe( CTopicValueEntry::ETopicsNamesIndex topicIndex )
{
    QString topicName = CTopicValueEntry::getTopicName( topicIndex );
    return unsubscribe(topicName);
}


/** ***********************************************************************
 * @fn setWillMessage     [thread safe]
 * @brief метод, задает will сообщение (отправляется в случае закрытия клиента)
 * @param CMqttMessage
 * @return bool
 ** ***********************************************************************/
bool CMqttClientHandler::setWillMessage(const CMqttMessage &message)
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
            QMetaObject::Connection conn = connect( this, &CMqttClientHandler::setWillMessageResult, [ &sema, &res ]( const bool state )
            {
                res = state;
                sema.release(1);
            });
            if ( ! QMetaObject::invokeMethod( this,
                                              "setWillMessageSlot",
                                              Qt::QueuedConnection,
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
        QMetaObject::Connection conn = connect( this, &CMqttClientHandler::getStateResult, [ &sema, &state ]( const ClientState clientState )
        {
            state = clientState;
            sema.release(1);
        });
        if ( ! QMetaObject::invokeMethod( this,
                                          "getStateSlot",
                                          Qt::QueuedConnection ) )
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
 * @fn localInstance    [static]
 * @brief метод, возвращает клиент, подключенный к локальному брокеру
 * @return CMqttClientHandler *
 ** ***********************************************************************/
CMqttClientHandler * CMqttClientHandler::localInstanceOld()
{
    if ( localClient.isNull() )
    {
        QMutexLocker locker( & instanceMutex );
        if ( localClient.isNull() )
        {
            QMutexLocker locker( & initHandlerMutex );
            localClientThread = new QThread{};
//    const QString localhost{ QHostAddress{ QHostAddress::LocalHost }.toString() };^M
//    m_ip = CSettings::readValue( MQTTSETT_GROUP + MQTTSETT_IP, localhost ).toString();^M
//    m_port = CSettings::readValue( MQTTSETT_GROUP + MQTTSETT_PORT, 1883 ).toUInt();^M
            localClient = new CMqttClientHandler(QHostAddress{ QHostAddress::LocalHost }.toString(),
                                                1883, QCoreApplication::applicationName());
            localClient.data()->moveToThread( localClientThread.data() );
            localClientThread.data()->start();
            connect( localClientThread.data(), & QThread::finished,
                     localClient.data(), & CMqttClientHandler::deleteLater );
        }
    }
    Q_ASSERT( localClient != nullptr );
    return localClient;
}

/** ***********************************************************************
 * @fn deinitLocalInstance      [static]
 * @brief метод, деинициализирует локальный mqtt клиент
 ** ***********************************************************************/
void CMqttClientHandler::deinitLocalInstanceOld()
{
    QMutexLocker locker( & initHandlerMutex );
    if ( ! localClient.isNull() )
    {
        if ( ! QMetaObject::invokeMethod( localClient.data(),
                                          "threadStopedSlot",
                                          Qt::BlockingQueuedConnection ) )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
        }
        localClientThread.data()->quit();
        localClientThread.data()->wait();
        delete localClientThread;
        localClientThread = nullptr;
//        delete localClient;
        localClient = nullptr;
    }
//    bool needDel = false;
//    {
//        QMutexLocker locker( & initHandlerMutex );
//        if ( ! localClient.isNull() )
//        {
//            if ( ! QMetaObject::invokeMethod( localClient.data(),
//                                              "threadStopedSlot",
//                                              Qt::BlockingQueuedConnection ) )
//            {
//                PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
//            }
//            localClientThread.data()->quit();
//            localClientThread.data()->wait();
//            needDel = true;
//        }
//    }
//    if ( needDel )
//    {
//        localClientThread.reset();
//    }
}

void CMqttClientHandler::destroyConnection(CMqttClientHandler *ptr)
{
    if ( ! QMetaObject::invokeMethod( ptr, "threadStopedSlot",
                                          Qt::BlockingQueuedConnection ) )
    {
            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
    }
    ptr->thread()->quit();
    ptr->thread()->wait();
    ptr->thread()->deleteLater();
}


/** ***********************************************************************
 * @fn startLocalInstance   [static]
 * @brief метод, запускает в работу MQTT клиент
 ** ***********************************************************************/
void CMqttClientHandler::startLocalInstanceOld()
{
    if ( ! QMetaObject::invokeMethod( localClient.data(),
                                    "threadStartedSlot",
                                    Qt::QueuedConnection ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно создать invokeMethod" );
    }
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
//CMqttClientHandler *CMqttClientHandler::createConnection(QThread * thread,
//                                                         const QString &host,
//                                                         quint16 port,
//                                                         const CMqttMessage & willMessage,
//                                                         QObject * parent )

//{
//    CMqttClientHandler * mqttClient = nullptr;
//    if ( thread == nullptr )
//    {
//        PRINT_CONSOLE_MESSAGE_ERROR( "Некорректный входной указатель на поток" );
//    }
//    else
//    {
//            mqttClient = new CMqttClientHandler( host, port, parent );
//            mqttClient->setWillMessage( willMessage );
//            QSemaphore sema(0);
//            auto conn1 = QObject::connect( mqttClient, &CMqttClientHandler::connected, [ &sema ]()
//            {
//                sema.release(1);
//            } );
//            auto conn2 = QObject::connect( mqttClient, &CMqttClientHandler::disconnected, [ &sema ]()
//            {
//                sema.release(1);
//            } );
//        if ( QThread::currentThread() == thread )
//        {
//            mqttClient->threadStartedSlot();
///*            if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
//            {
//                PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
//            }
//            QObject::disconnect( conn1 );
//            QObject::disconnect( conn2 );
//*/
//        }
//        else
//        {
///*            mqttClient = new CMqttClientHandler( host, port );
//            mqttClient->setWillMessage( willMessage );
//            mqttClient->moveToThread( thread );
//            QSemaphore sema(0);
//            auto conn1 = QObject::connect( mqttClient, &CMqttClientHandler::connected, [ &sema ]()
//            {
//                sema.release(1);
//            } );
//            auto conn2 = QObject::connect( mqttClient, &CMqttClientHandler::disconnected, [ &sema ]()
//            {
//                sema.release(1);
//            } );*/
//            if ( thread->isRunning() )
//            {
//                if ( ! QMetaObject::invokeMethod( mqttClient,
//                                                  "threadStartedSlot",
//                                                  Qt::QueuedConnection ) )
//                {
//                    PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
//                }
//            }
//            else
//            {
////                PRINT_CONSOLE_MESSAGE_INFO( "DEBUG: Поток не запущен. Запускаем поток и инициируем подключение клиента" );
//                QObject::connect( thread, &QThread::started, mqttClient, &CMqttClientHandler::threadStartedSlot );
//                thread->start();
//            }
//        }
//            if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
//            {
//                PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
//            }
//            QObject::disconnect( conn1 );
//            QObject::disconnect( conn2 );
//    }
//    return mqttClient;
//}


CMqttClientHandler *CMqttClientHandler::createConnection(const QString &host,
                                                         quint16 port,
                                                         const QString &clientId,
                                                         const CMqttMessage & willMessage,
                                                         QObject * parent )

{
    CMqttClientHandler * mqttClient = nullptr;
    QThread * connThread{ new QThread( parent ) };
    mqttClient = new CMqttClientHandler( host, port, clientId, parent );
//    mqttClient->setClientId(clientId);
    mqttClient->setWillMessage( willMessage );
    mqttClient->moveToThread( connThread );
    QSemaphore sema(0);
    auto conn1 = QObject::connect( mqttClient, &CMqttClientHandler::connected, [ &sema ]()
    {
        sema.release(1);
    } );
    auto conn2 = QObject::connect( mqttClient, &CMqttClientHandler::disconnected, [ &sema ]()
    {
        sema.release(1);
    } );
    QObject::connect( connThread, &QThread::started, mqttClient, &CMqttClientHandler::threadStartedSlot );
//    QObject::connect( mqttClient, &CMqttClientHandler::destroyed, &CMqttClientHandler::threadStopedSlot );
    connThread->start();

    if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
    }
    QObject::disconnect( conn1 );
    QObject::disconnect( conn2 );
    return mqttClient;
}

CMqttClientHandler *CMqttClientHandler::createConnection2(const QString &host, quint16 port, const QString &clientId, const QString &user, const QString &password, const CMqttMessage &willMessage, QObject *parent)
{
    CMqttClientHandler * mqttClient = nullptr;
    QThread * connThread{ new QThread( parent ) };
    mqttClient = new CMqttClientHandler( host, port, clientId, user, password, parent );
//    mqttClient->setClientId(clientId);
    mqttClient->setWillMessage( willMessage );
    mqttClient->moveToThread( connThread );
    QSemaphore sema(0);
    auto conn1 = QObject::connect( mqttClient, &CMqttClientHandler::connected, [ &sema ]()
    {
        sema.release(1);
    } );
    auto conn2 = QObject::connect( mqttClient, &CMqttClientHandler::disconnected, [ &sema ]()
    {
        sema.release(1);
    } );
    QObject::connect( connThread, &QThread::started, mqttClient, &CMqttClientHandler::threadStartedSlot );
//    QObject::connect( mqttClient, &CMqttClientHandler::destroyed, &CMqttClientHandler::threadStopedSlot );
    connThread->start();

    if ( ! sema.tryAcquire( 1, NORM_WAITING_TIMEOUT ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
    }
    QObject::disconnect( conn1 );
    QObject::disconnect( conn2 );
    return mqttClient;
}










