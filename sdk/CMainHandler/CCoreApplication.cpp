#include "CCoreApplication.h"
#include <CLoggerHandler/CLoggerHandler.h>
#include <CSettingsHandler/CSettingsHandler.h>
#include <CUtils/CUtils.h>
#include <CSettings/CSettings.h>
#include <CrashHandler/CrashHandler.h>


#ifdef USE_KEEP_ALIVE_HANDLER
    #include <CKeepAliveHandler/CKeepAliveHandler.h>
#endif
#ifdef USE_MQTT_TOPICS
    #include <CMqttTopics/CMqttTopicsHandler.h>
#endif

#include <QtCore/QDebug>
#include <QtNetwork/QHostAddress>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QJsonDocument>
#include <QtCore/QVariant>
#include <QtCore/QSemaphore>

static const char * MQTTSETT_GROUP = "";
static const char * MQTTSETT_IP = "MqttBrokerIp";
static const char * MQTTSETT_PORT = "MqttBrokerPort";
static const char * DBSETT_GROUP = "";
static const char * DBSETT_IP = "DatabaseIp";
static const char * DBSETT_PORT = "DatabasePort";
static const char * DBSETT_USER = "DatabaseUser";
static const char * DBSETT_PASSWORD = "DatabasePassword";

/** ***********************************************************************
 * @fn [конструктор]
 ** ***********************************************************************/
CCoreApplication::CCoreApplication( QObject * parent ) :
    QObject( parent ),
    m_moduleName( QCoreApplication::applicationName() ),
    m_settingsOwner( QCoreApplication::applicationName() )
{
#ifdef USE_MQTT_TOPICS
    qRegisterMetaType< CMqttTopic >("CMqttTopic");
#endif
    CrashHandler::getInstance();
}

#ifdef USE_MQTT_HANDLER
bool CCoreApplication::publishToMqttTopic( const QString &connName,
                                           const CMqttTopic * topic,
                                           bool retain/* = false*/ )
{
    CMqttClientHandler *conn = getMqttConnection( connName );
    if ( conn != nullptr && topic != nullptr )
    {
        return conn->safePublish( topic->buildMessage( retain ) );
    }
    return false;
}

bool CCoreApplication::publishToMqttTopic( const CMqttTopic * topic,
                                           bool retain/* = false*/ )
{
    return publishToMqttTopic( getLocalMqttConnectionName(),
                               topic,
                               retain );
}

bool CCoreApplication::publishToMqttTopic( const CMqttTopic & topic,
                                           bool retain/* = false*/ )
{
    return publishToMqttTopic( getLocalMqttConnectionName(),
                               &topic,
                               retain );
}

bool CCoreApplication::publishToMqttTopic(const QString & topicName,
                                            const QJsonObject &obj, bool retain)
{
    return publishMqttMsg(getLocalMqttConnectionName(), CMqttMessage(topicName, obj, retain));
}

/*bool CCoreApplication::publishToMqttTopic(const QString & topicName,
                                            const QJsonDocument &data, bool retain)
{
    return publishMqttMsg(getLocalMqttConnectionName(),
                          CMqttMessage(topicName, data.toJson(), retain));
}
*/

bool CCoreApplication::publishToMqttTopic(const QString & topicName,
                                            const QByteArray &data, bool retain)
{
    return publishMqttMsg(getLocalMqttConnectionName(), CMqttMessage(topicName, data, retain));
}


bool CCoreApplication::publishToMqttTopic( CTopicValueEntry::ETopicsNamesIndex topicIndex,
                    const QJsonObject &obj, bool retain)
{
    return publishMqttMsg(getLocalMqttConnectionName(), CMqttMessage(topicIndex, obj, retain));
}

bool CCoreApplication::publishToMqttTopic( CTopicValueEntry::ETopicsNamesIndex topicIndex,
                    const QByteArray &data, bool retain)
{
    return publishMqttMsg(getLocalMqttConnectionName(), CMqttMessage(topicIndex, data, retain));
}


bool CCoreApplication::publishMqttMsg(const QString &connName, const CMqttMessage & message)
{
    CMqttClientHandler *conn = getMqttConnection(connName);
    if(conn)
    {
        return conn->safePublish( message );
    }
    return false;
}

#endif

/** ***********************************************************************
 * @fn attach
 ** ***********************************************************************/
int CCoreApplication::attach()
{
    int result = 0;
    PRINT_CONSOLE_MESSAGE_INFO( QString( "Starting controller %1" ).arg(metaObject()->className()) );

    loadSystemSettings();
#ifdef USE_MQTT_HANDLER
    CMqttClientHandler *localMqttClient =
            createMqttConnection( getLocalMqttConnectionName(),
                                  getSystemSetting("mqtt_host"),
                                  static_cast< uint16_t >( getSystemSetting("mqtt_port").toUInt() ) );
#endif
/*! Запуск логгера */
#ifdef USE_MQTT_HANDLER
//    CLoggerHandler::getInstance( localMqttClient );
    CMqttTopicsHandler::getInstance( localMqttClient );
//#else
//    CLoggerHandler::getInstance();
#endif

#ifdef USE_KEEP_ALIVE_HANDLER
/* запуск keepalive */
    CKeepAliveHandler::getInstance( localMqttClient );
    m_keepaliveTimer = new QTimer( qApp );
    m_keepaliveTimer->setInterval( CKeepAliveHandler::KEEPALIVE_TIMEOUT_RECOMMEND );
    QObject::connect( m_keepaliveTimer, & QTimer::timeout, this, [this]()
    {
        SEND_KEEPALIVE_MESSAGE(m_moduleName);
    }, Qt::QueuedConnection );

    m_keepaliveTimer->start();
#endif

/*
    TODO: need to init database handler
*/
/* Установка настроек по-умолчанию */
    CSettingsHandler    *settingsHandler = CSettingsHandler::getInstance();
    QList<CSettingsEntry> defSettingsList;
    bool needToOverride = getDefaultAppSettings(defSettingsList);
    if ( ! defSettingsList.isEmpty() )
    {
        settingsHandler->setSettingsList(defSettingsList, needToOverride);
    }    

/* Загрузка сохраненных настроек */
    QList < CSettingsEntry > currSettingsList = settingsHandler->getAllSettings( true, m_settingsOwner );    
    applyAppSettings(currSettingsList);

/* Запуск основного цикла */
    result = onAttach();
    if(result == 0)
    {    
        PRINT_CONSOLE_MESSAGE_INFO( QString( "Started controller %1" ).arg(metaObject()->className()) );
    }
    return result;
}

/** ***********************************************************************
 * @fn detach
 ** ***********************************************************************/
int CCoreApplication::detach()
{
    int result;

    QString className = metaObject()->className();
    PRINT_CONSOLE_MESSAGE_INFO( "Stopping controller "  + className );
    result = onDetach();
    PRINT_CONSOLE_MESSAGE_INFO( "Stopped controller " + className );


/*#if 0

#ifdef USE_MQTT_HANDLER
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited local mqtt connection" );
    CMqttClientHandler::deinitLocalInstance();
#endif
#endif
*/
    CSettingsHandler::deinitSettingsHandler();
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited settings handler" );

#ifdef USE_KEEP_ALIVE_HANDLER
    m_keepaliveTimer->stop();
    CKeepAliveHandler::deinitHandler();
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited keepalive handler" );
#endif

//    CLoggerHandler::deinitLoggerHandler();
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited logger handler" );

    PRINT_CONSOLE_MESSAGE_INFO( "Deinited console logger" );
    CConsoleLogger::deinitLoggerHandler();

#ifdef USE_MQTT_HANDLER
    CMqttTopicsHandler::deinitHandler();
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited mqtt topics handler" );

    foreach (const QString &connName, m_mqttConnections.keys())
    {
        PRINT_CONSOLE_MESSAGE_INFO( "Destroy MQTT client '"  + connName +"'");
        m_mqttConnectionsMutex.lock();
        CMqttClientHandler::destroyConnection(m_mqttConnections[connName]);
        m_mqttConnections.remove(connName);
        m_mqttConnectionsMutex.unlock();
    }
#endif

    return result;
}

void CCoreApplication::onSubscribeToTopic(CMqttTopic *topic)
{
    subscribeToMqttTopic(topic);
}

void CCoreApplication::onSubscribeToTopicByName(const QString &topic)
{
    subscribeToMqttTopic(getMqttConnection(getLocalMqttConnectionName()), topic);
}

bool CCoreApplication::getDefaultAppSettings(QList<CSettingsEntry> &list)
{
    Q_UNUSED(list);
    return false;
}

void CCoreApplication::applyAppSettings(const QList < CSettingsEntry > &list)
{
    Q_UNUSED(list);
}

bool CCoreApplication::loadSystemSettings()
{
    QString group(DBSETT_GROUP);
    QString mqttGroup(MQTTSETT_GROUP);
    QString localhost = QHostAddress(QHostAddress::LocalHost).toString();
    // TODO Load from File
    m_systemSettings["db_host"] =
            CSettings::readValue( group + DBSETT_IP, localhost ).toString();
    m_systemSettings["db_port"] =
            QString::number( CSettings::readValue( group + DBSETT_PORT, 3306 ).toUInt() );
    m_systemSettings["db_name"] = "mntconti_database";
    m_systemSettings["db_user"] =
            CSettings::readValue( group + DBSETT_USER,  "mntconti" ).toString();
    m_systemSettings["db_password"] =
            CSettings::readValue( group + DBSETT_PASSWORD,  "18iTnOcTnM81" ).toString();
    m_systemSettings["mqtt_host"] =
            CSettings::readValue( mqttGroup + MQTTSETT_IP, localhost ).toString();
    m_systemSettings["mqtt_port"] =
            QString::number( CSettings::readValue( mqttGroup + MQTTSETT_PORT, 1883 ).toUInt() );
    return true;
}

#ifdef USE_MQTT_HANDLER
CMqttClientHandler *CCoreApplication::createMqttConnection(const QString &connName, 
                                                           const QString &host,
                                                           quint16 port)
{
    PRINT_CONSOLE_MESSAGE_INFO( QString("Create new MQTT connection '%1' for %2:%3").
                                arg(connName).arg(host).arg(port));
    
    CMqttClientHandler *ptr = getMqttConnection(connName);
 
    m_mqttConnectionsMutex.lock();
    if(ptr == nullptr)
    {
        ptr = CMqttClientHandler::createConnection( host, port, m_moduleName );
        m_mqttConnections[connName] = ptr;
    }
    m_mqttConnectionsMutex.unlock();

    return ptr;
}

CMqttClientHandler *CCoreApplication::createMqttConnection2(const QString &connName, const QString &host, quint16 port, const QString &user, const QString &password)
{
    PRINT_CONSOLE_MESSAGE_INFO( QString("Create new MQTT connection '%1' for %2:%3 with user %4").
                                arg(connName).arg(host).arg(port).arg(user));

    CMqttClientHandler *ptr = getMqttConnection(connName);

    m_mqttConnectionsMutex.lock();
    if(ptr == nullptr)
    {
        ptr = CMqttClientHandler::createConnection2(host, port, m_moduleName, user, password);
        m_mqttConnections[connName] = ptr;
    }
    m_mqttConnectionsMutex.unlock();

    return ptr;
}

/** ***********************************************************************
 * @fn getMqttConnection
 ** ***********************************************************************/
CMqttClientHandler *CCoreApplication::getMqttConnection(const QString &connName)
{
    QMutexLocker   m(&m_mqttConnectionsMutex);
    if(m_mqttConnections.contains(connName))
    {
        return m_mqttConnections[connName];
    }
    return nullptr;
}

/** ***********************************************************************
 * @fn deleteMqttConnection
 ** ***********************************************************************/
void CCoreApplication::deleteMqttConnection( const QString & connName, const bool deleteConnection )
{
    QMutexLocker   m(&m_mqttConnectionsMutex);
    if ( m_mqttConnections.contains( connName ) )
    {
        CMqttClientHandler * connection{ m_mqttConnections.value( connName ) };
        m_mqttConnections.remove( connName );
        if ( deleteConnection && connection != nullptr )
        {
//            if ( ! connection->forceDisconnect() )
//            {
//                PRINT_LOG_MESSAGE_WARN( "MQTT connection [" + connName
//                                        + "] closed with errors!" );
//            }
            CMqttClientHandler::destroyConnection( connection );
            connection->deleteLater();
        }
    }
}

void CCoreApplication::onMqttMessageReceived(const QString &topicName, const QByteArray &payload)
{
    Q_UNUSED(topicName)
    Q_UNUSED(payload)
}

/** ***********************************************************************
 * @fn subscribeToMqttTopic
 ** ***********************************************************************/
bool CCoreApplication::subscribeToMqttTopic(CMqttClientHandler *conn, const QString &topicName)
{
    auto state = conn->subscribe( topicName );
    bool result = (state == CMqttClientHandler::SubscriptionState::Subscribed ||
                     state == CMqttClientHandler::SubscriptionState::SubscriptionPending);
    if ( result )
    {
        PRINT_CONSOLE_MESSAGE_INFO( QString( "Succesfully subscribed to MQTT topic '%1'" ).arg(topicName) );
    }
    else
    {
            PRINT_CONSOLE_MESSAGE_WARN( QString( "Can't subscribed to MQTT topic '%1'" ).arg(topicName) );
    }

    return result;
}

/** ***********************************************************************
 * @fn subscribeToMqttTopic
 ** ***********************************************************************/
void CCoreApplication::processReceivedMqttMessage( const CMqttMessage & message )
{
    m_mqttTopicsMutex.lockForRead();
    QList < QPointer < CMqttTopic > > topics{ m_mqttTopics.values( message.getTopicName() ) };
    m_mqttTopicsMutex.unlock();
    if ( ! topics.isEmpty() )
    {
        for ( auto i : topics )
        {
            if ( i.isNull() )
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Обнаружен некорректный объект топика ["
                                            + message.getTopicName() + "]" );
            }
            else
            {
                i.data()->initFromData( message.getMessage() );
                emit i.data()->messageReceived();
            }
        }
    }
    else
    {
        onMqttMessageReceived(message.getTopicName(), message.getMessage());
    }
}

/** ***********************************************************************
 * @fn subscribeToMqttTopic
 ** ***********************************************************************/
bool CCoreApplication::subscribeToMqttTopic( CMqttTopic * topic )
{
    return subscribeToMqttTopic( getLocalMqttConnectionName(), topic );
}

bool CCoreApplication::subscribeToMqttTopic( const QString &connName,
                                             CMqttTopic * topic )
{
    CMqttClientHandler *conn = getMqttConnection( connName );
    if ( conn != nullptr )
    {
        m_mqttTopicsMutex.lockForWrite();
        if ( ! m_connectionStatus )
        {
            m_connectionStatus = true;
            connect( conn, &CMqttClientHandler::messageReceived,
                     this, & CCoreApplication::processReceivedMqttMessage, Qt::QueuedConnection );
        }
        bool existance{ m_mqttTopics.contains( topic->topicName() ) };
        topic->setMqttConnectionName( connName );
        m_mqttTopics.insertMulti( topic->topicName(), topic );
        m_mqttTopicsMutex.unlock();
        if ( ! existance )
        {
            if ( ! subscribeToMqttTopic( conn, topic->topicName() ) )
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Не удалось подписаться на топик " + topic->topicName() );
            }
        }
        return true;
    }
    return false;
}

#pragma message "Здесь необходимо отслеживать от какого именно connectionName нужно отписаться. \
На данный момент логика работы построена только на локальный коннект"

bool CCoreApplication::unsubscribeFromMqttTopic( const QString & connName, CMqttTopic *topic )
{
    m_mqttTopicsMutex.lockForWrite();
    QList < QPointer < CMqttTopic > > topics{ m_mqttTopics.values( topic->topicName() ) };
    if ( ! topics.isEmpty() )
    {
        for ( auto i : topics )
        {
            if ( i.isNull() )
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Обнаружен некорректный объект топика ["
                                            + topic->topicName() + "]" );
            }
            else
            {
                if ( i == topic
                     && i.data()->mqttConnectionName() == connName )
                {
                    PRINT_CONSOLE_MESSAGE_INFO( "Найден топик[" + topic->topicName() + "] для отписки, connectionName["
                                                + connName + "]" );
                    m_mqttTopics.remove( topic->topicName(), topic );

                    if ( ! m_mqttTopics.contains( topic->topicName() ) )
                    {
                        unsubscribeFromMqttTopic( connName, topic->topicName() );
                    }
                    m_mqttTopicsMutex.unlock();
                    return true;
                }
            }
        }
    }
    PRINT_CONSOLE_MESSAGE_WARN( "Не найден топик[" + topic->topicName() + "] для отписки, connectionName["
                                + connName + "]" );
    m_mqttTopicsMutex.unlock();
    return false;
}

bool CCoreApplication::unsubscribeFromMqttTopic( CMqttTopic *topic )
{
    return unsubscribeFromMqttTopic(getLocalMqttConnectionName(), topic);
}

bool CCoreApplication::unsubscribeFromMqttTopic(const QString &topicName)
{
    return unsubscribeFromMqttTopic(getLocalMqttConnectionName(), topicName);
}

bool CCoreApplication::unsubscribeFromMqttTopic(const QString &connName, const QString &topicName)
{
    CMqttClientHandler *conn = getMqttConnection(connName);
    if(conn)
    {
        return conn->unsubscribe(topicName);
    }
    return false;
}

#if 0
CMqttTopicConnection *CCoreApplication::getMqttTopicConnection(const QString &connName, 
                                                                const QString &topicName)
{
    CMqttClientHandler *mqttClientHandler = getMqttConnection(connName);
    if(mqttClientHandler == nullptr)
    {
        PRINT_CONSOLE_MESSAGE_ERROR("MQTT connection with name '" + connName + "' not found");
        return nullptr;
    }    

    CMqttTopicConnection * connection = CMqttTopicsHandler::createConnection(
                mqttClientHandler,
                topicName,
                CMqttTopicConnection::EConnectionDirection::Connection_fullDuplex );
    if ( connection == nullptr )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Unable to create/retreive connection for topic '" 
                                    + topicName + "' on '" + connName + "' connection" );
        return nullptr;
    }
    
    return connection;
}
#endif


#endif

const QString CCoreApplication::getSystemSetting(const QString &keyName) const
{
    if(m_systemSettings.contains(keyName))
    {
        return m_systemSettings[keyName];
    }
    return "";
}

