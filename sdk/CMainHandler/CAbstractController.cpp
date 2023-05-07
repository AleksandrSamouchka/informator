#include <QDebug>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QJsonDocument>
#include <QtCore/QVariant>
#include <QtCore/QSemaphore>

#include <CLoggerHandler/CLoggerHandler.h>

#ifdef USE_KEEP_ALIVE_HANDLER
    #include <CKeepAliveHandler/CKeepAliveHandler.h>
#endif

#include <CSettingsHandler/CSettingsHandler.h>

#ifdef USE_MQTT_TOPICS
    #include <CMqttTopics/CMqttTopicsHandler.h>
#endif

#include <CUtils/CUtils.h>

#include "CAbstractController.h"

CAbstractController::CAbstractController( QObject * parent ) :
    QObject( parent ),
#ifdef USE_MQTT_HANDLER
    m_mqttClient( CMqttClientHandler::localInstanceOld() ),
#endif
    m_moduleName( QCoreApplication::applicationName() ),
    m_settingsOwner( QCoreApplication::applicationName() )
{
#ifdef USE_MQTT_HANDLER
    connect( m_mqttClient, & CMqttClientHandler::connected, this, & CAbstractController::mqttConnected );
    connect( m_mqttClient, & CMqttClientHandler::disconnected, this, & CAbstractController::mqttDisconnected );
    connect( m_mqttClient, & CMqttClientHandler::messageReceived, this, & CAbstractController::mqttMessageReceived );
#endif
}
/*
bool CAbstractController::publishToMqttTopic( const QString & topicName,
                                              const QJsonDocument &data,
                                              bool retain,
                                              quint8 qos )
{
#ifdef USE_MQTT_HANDLER
//    return m_mqttClient->safePublish(CMqttMessage{ topicName, data.toJson(), qos, retain });
    return publishMqttMsg(CMqttMessage(topicName, data.toJson(), qos, retain));
#else
    Q_UNUSED(topicName);
    Q_UNUSED(data);
    Q_UNUSED(retain);
    Q_UNUSED(qos);

    return true;
#endif
}

bool CAbstractController::publishToMqttTopic( const QString & topicName,
                                              const QJsonObject &obj,
                                              bool retain,
                                              quint8 qos )
{
#ifdef USE_MQTT_HANDLER
//    return m_mqttClient->safePublish(CMqttMessage{ topicName, obj, qos, retain });
    return publishMqttMsg(CMqttMessage(topicName, obj, qos, retain));
#else
    Q_UNUSED(topicName);
    Q_UNUSED(obj);
    Q_UNUSED(retain);
    Q_UNUSED(qos);

    return true;
#endif
}


bool CAbstractController::publishToMqttTopic( const QString & topicName,
                                              const QByteArray &data,
                                              bool retain,
                                              quint8 qos )
{
#ifdef USE_MQTT_HANDLER
//    return m_mqttClient->safePublish(CMqttMessage{ topicName, data, qos, retain });
    return publishMqttMsg(CMqttMessage(topicName, data, qos, retain));
#else
    Q_UNUSED(topicName);
    Q_UNUSED(data);
    Q_UNUSED(retain);
    Q_UNUSED(qos);

    return true;
#endif
}
*/
#ifdef USE_MQTT_HANDLER
/*bool CAbstractController::publishToMqttTopic(CTopicValueEntry::ETopicsNamesIndex topicIndex,
                            const QJsonDocument &data,
                            bool retain, quint8 qos)
{
//#ifdef USE_MQTT_HANDLER
    return publishMqttMsg(CMqttMessage(topicIndex, data.toJson(), qos, retain));
//#else
//    Q_UNUSED(topicIndex);
//    Q_UNUSED(data);
//    Q_UNUSED(retain);
//    Q_UNUSED(qos);

//    return true;
//#endif
}

bool CAbstractController::publishToMqttTopic(CTopicValueEntry::ETopicsNamesIndex topicIndex,
                            const QJsonObject &obj,
                            bool retain, quint8 qos)
{
//#ifdef USE_MQTT_HANDLER
    return publishMqttMsg(CMqttMessage(topicIndex, obj, qos, retain));
//#else
//    Q_UNUSED(topicIndex);
//    Q_UNUSED(obj);
//    Q_UNUSED(retain);
//    Q_UNUSED(qos);

//    return true;
//#endif
}

bool CAbstractController::publishToMqttTopic(CTopicValueEntry::ETopicsNamesIndex topicIndex,
                            const QByteArray &data,
                            bool retain, quint8 qos)
{
//#ifdef USE_MQTT_HANDLER
    return publishMqttMsg(CMqttMessage(topicIndex, data, qos, retain));
//#else
//    Q_UNUSED(topicIndex);
//    Q_UNUSED(data);
//    Q_UNUSED(retain);
//    Q_UNUSED(qos);
//
//    return true;
//#endif
}
*/

//#ifdef USE_MQTT_HANDLER
bool CAbstractController::publishMqttMsg(const CMqttMessage & message)
{
    return m_mqttClient->safePublish( message );
}
#endif

bool CAbstractController::subscribeToMqttTopic( const QString & topicName )
{
#ifdef USE_MQTT_HANDLER
    const auto state{ m_mqttClient->subscribe( topicName ) };
    const auto result{ state == CMqttClientHandler::SubscriptionState::Subscribed ||
                       state == CMqttClientHandler::SubscriptionState::SubscriptionPending };
    if ( result )
    {
        PRINT_CONSOLE_MESSAGE_INFO( QString( "Succesfully subscribed to MQTT topic '%1'" ).arg(topicName) );
    }
    else
    {
        PRINT_CONSOLE_MESSAGE_WARN( QString( "Can't subscribed to MQTT topic '%1'" ).arg(topicName) );
    }
    return result;
#else
    Q_UNUSED(topicName);
    return true;
#endif
}

bool CAbstractController::unsubscribeFromMqttTopic( const QString & topicName ) 
{
#ifdef USE_MQTT_HANDLER
    return m_mqttClient->unsubscribe( topicName );
#else
    Q_UNUSED(topicName);
    return true;
#endif
}

#ifdef USE_MQTT_HANDLER
bool CAbstractController::subscribeToMqttTopic( CTopicValueEntry::ETopicsNamesIndex topicIndex )
{
    const auto state{ m_mqttClient->subscribe( topicIndex ) };
    const auto result{ state == CMqttClientHandler::SubscriptionState::Subscribed ||
                       state == CMqttClientHandler::SubscriptionState::SubscriptionPending };
    if ( result )
    {
        PRINT_CONSOLE_MESSAGE_INFO( QString( "Succesfully subscribed to MQTT topic '%1' using index %2" )
                                    .arg(CTopicValueEntry::getTopicName( topicIndex ))
                                    .arg(static_cast<int>( topicIndex )) );
    }
    else
    {
        PRINT_CONSOLE_MESSAGE_WARN( QString( "Can't subscribed to MQTT topic '%1' using index %2" )
                                    .arg(CTopicValueEntry::getTopicName( topicIndex ))
                                    .arg(static_cast<int>( topicIndex )) );
    }
    return result;
}

bool CAbstractController::unsubscribeFromMqttTopic( CTopicValueEntry::ETopicsNamesIndex topicIndex )
{
    return m_mqttClient->unsubscribe(topicIndex);
}
#endif

/*

    TODO Need to remove in future

*/

void CAbstractController::OnMqttMessageReceived( const QString & topic,
                                       const QJsonObject & json )
{
    if ( topic == "_COMMON_" )
    {
        const auto action{ json.value( "action" ) };

        if ( action == "shutdown" )
        {
            QCoreApplication::exit( 0 );
        }
    }
}

#ifdef USE_MQTT_HANDLER
void CAbstractController::OnMqttMessageReceived( CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                        const QJsonObject & json )
{
    OnMqttMessageReceived(CTopicValueEntry::getTopicName( topicIndex ), json);
}
#endif


QByteArray CAbstractController::makeSimpleJson( const QVariant & value )
{
    return QJsonDocument{ QJsonObject{ qMakePair( QByteArrayLiteral( "Value" ),
                                                  QJsonValue::fromVariant( value ) ) } }.toJson();
}

/** ***********************************************************************
 * @fn attach
 ** ***********************************************************************/
void CAbstractController::attach()
{
    PRINT_CONSOLE_MESSAGE_INFO( QString( "Starting controller %1" ).arg(metaObject()->className()) );
#ifdef USE_MQTT_HANDLER
/* Запуск mqtt клиента */
    QSemaphore sema( 0 );
    auto conn1 = connect( m_mqttClient, & CMqttClientHandler::connected, [ &sema ]()
    {
        sema.release( 1 );
    });
    auto conn2 = connect( m_mqttClient, & CMqttClientHandler::disconnected, [ &sema ]()
    {
        sema.release( 1 );
    });
    CMqttClientHandler::startLocalInstanceOld();
    if ( ! sema.tryAcquire( 1, 30000 ) )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Таймаут ожидания исполнения метода" );
    }
    disconnect( conn1 );
    disconnect( conn2 );
#endif

/*! Запуск логгера */
//#ifdef USE_MQTT_HANDLER
//    CLoggerHandler::getInstance(CMqttClientHandler::localInstanceOld());
//#else
//    CLoggerHandler::getInstance();
//#endif

#ifdef USE_KEEP_ALIVE_HANDLER
/* запуск keepalive */
    CKeepAliveHandler::getInstance(CMqttClientHandler::localInstanceOld());
    m_keepaliveTimer = new QTimer( qApp );
    m_keepaliveTimer->setInterval( CKeepAliveHandler::KEEPALIVE_TIMEOUT_RECOMMEND );
    QObject::connect( m_keepaliveTimer, & QTimer::timeout, this, [this]()
    {
        SEND_KEEPALIVE_MESSAGE(m_moduleName);
    }, Qt::QueuedConnection );

    m_keepaliveTimer->start();
#endif

/* Установка настроек по-умолчанию */
    CSettingsHandler    *settingsHandler = CSettingsHandler::getInstance();
    QList<CSettingsEntry> defSettingsList;
    bool needToOverride = getDefaultSettings(defSettingsList);
    if ( ! defSettingsList.isEmpty() )
    {
        settingsHandler->setSettingsList(defSettingsList, needToOverride);
    }    
    
/* Загрузка сохраненных настроек */
    QList < CSettingsEntry > currSettingsList = settingsHandler->getAllSettings( true, m_settingsOwner );    
    loadSettings(currSettingsList);

/* Запуск основного цикла */
    onAttach();
    PRINT_CONSOLE_MESSAGE_INFO( QString( "Started controller %1" ).arg(metaObject()->className()) );

}

/** ***********************************************************************
 * @fn detach
 ** ***********************************************************************/
void CAbstractController::detach()
{
    QString className = metaObject()->className();
    PRINT_CONSOLE_MESSAGE_INFO( "Stopping controller "  + className );
    onDetach();
    PRINT_CONSOLE_MESSAGE_INFO( "Stopped controller " + className );
#ifdef USE_MQTT_HANDLER
//    m_mqttClient->thread()->quit();
//    m_mqttClient->thread()->wait( timeoutWaitThreadQuits );
//    PRINT_CONSOLE_MESSAGE_INFO( "Closed mqtt connection" );
#endif
//    CLoggerHandler::deinitLoggerHandler();
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited logger handler" );
    CSettingsHandler::deinitSettingsHandler();
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited settings handler" );
#ifdef USE_MQTT_TOPICS
    CMqttTopicsHandler::deinitHandler();
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited mqtt topics handler" );
#endif
#ifdef USE_KEEP_ALIVE_HANDLER
    m_keepaliveTimer->stop();
    CKeepAliveHandler::deinitHandler();
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited keepalive handler" );
#endif
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited console logger" );
    CConsoleLogger::deinitLoggerHandler();

#ifdef USE_MQTT_HANDLER
    PRINT_CONSOLE_MESSAGE_INFO( "Deinited local mqtt connection" );
    CMqttClientHandler::deinitLocalInstanceOld();
#endif
}

/** ***********************************************************************
 * @fn mqttConnected
 ** ***********************************************************************/
void CAbstractController::mqttConnected()
{
    PRINT_CONSOLE_MESSAGE_INFO( "Succesfully connected to MQTT" );
}

/** ***********************************************************************
 * @fn mqttDisconnected
 ** ***********************************************************************/
void CAbstractController::mqttDisconnected()
{
    PRINT_CONSOLE_MESSAGE_INFO( "Succesfully disconnected from MQTT" );
}

/** ***********************************************************************
 * @fn mqttMessageReceived
 ** ***********************************************************************/
#ifdef USE_KEEP_ALIVE_HANDLER
void CAbstractController::mqttMessageReceived( const CMqttMessage & message )
{
    QJsonParseError error{};
    const auto json{ QJsonDocument::fromJson( message.getMessage(), & error ).object() };

//    PRINT_CONSOLE_MESSAGE_INFO( QString( "Incoming message %1" ).arg( QString(message.getMessage()) ) );

    if ( error.error != QJsonParseError::NoError )
    {
//        PRINT_CONSOLE_MESSAGE_WARN( QString( "Failed to parse json: %1 (%2)" )
//                                    .arg(error.errorString())
//                                    .arg( QString(message.getMessage()))
//                                    );
    }
    else
    {
        CTopicValueEntry::ETopicsNamesIndex topicIndex = message.getTopicIndex();
        if(topicIndex ==  CTopicValueEntry::ETopicsNamesIndex::Topic_unknown)
        {
            OnMqttMessageReceived( message.getTopicName(), json );
        }
        else 
        {
            OnMqttMessageReceived( message.getTopicIndex(), json );
        }
    }
}
#endif

bool CAbstractController::getDefaultSettings(QList<CSettingsEntry> &list)
{
    Q_UNUSED(list)
    return false;
}

void CAbstractController::loadSettings(QList < CSettingsEntry > &list)
{
    Q_UNUSED(list)
}

