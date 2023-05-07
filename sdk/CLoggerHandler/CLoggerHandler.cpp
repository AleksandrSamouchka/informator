#include "CLoggerHandler.h"

#include <QtCore/QSemaphore>
#include <QtCore/QTimer>

const int CLoggerHandler::LOG_MESSAGE_QUEUE_SIZE = 10000;
const int CLoggerHandler::CREATE_MQTT_CLIENT_TIMEOUT = 10000;
QMutex CLoggerHandler::initHandlerMutex{};
QMutex CLoggerHandler::instanceMutex{};
QPointer<CLoggerHandler> CLoggerHandler::handler{ nullptr };

/** ***********************************************************************
 * @fn deinitDatabaseHandler
 * @brief статический метод для деинициализации singleton объекта
 * обработчика запросов в БД
 * @param none
 * @return none
 ** ***********************************************************************/
void CLoggerHandler::deinitLoggerHandler( )
{
    QMutexLocker locker( & initHandlerMutex );
    if ( handler.isNull() )
    {
        return;
    }
    delete handler;
}

/** ***********************************************************************
 * @fn методы, возвращающие наименования топиков    [static]
 ** ***********************************************************************/
QString CLoggerHandler::getLoggerTopicName()
{
/* TODO Switch to topic index */
    static const QString topic{ "LOGGER/LOG_MESSAGE" };
    return topic;
}

QString CLoggerHandler::getGuiMessageTopicName()
{
/* TODO Switch to topic index */
    static const QString topic{ "LOGGER/GUI_MESSAGE" };
    return topic;
}

/** ***********************************************************************
 * @fn CLoggerHandler  [конструктор]
 ** ***********************************************************************/
#ifdef USE_MQTT_HANDLER
CLoggerHandler::CLoggerHandler( CMqttClientHandler * mqttClient, QObject *parent) :
    QObject(parent),
    m_mqttClient(mqttClient)
{
    Q_ASSERT( mqttClient != nullptr );
    QMutexLocker locker( & initHandlerMutex );
    static auto i = qRegisterMetaType< CLoggerMessage >("CLoggerMessage");
    Q_UNUSED( i );
}
#endif
CLoggerHandler::CLoggerHandler( QObject *parent ) :
    QObject(parent)
//,
//    m_mqttClient(nullptr)
{
    QMutexLocker locker( & initHandlerMutex );
    qRegisterMetaType< CLoggerMessage >("CLoggerMessage");
}

/** ***********************************************************************
 * @fn addLogMessageSlot
 * @brief слот для отправки LOG сообщений в соответствующий MQTT топик
 * @param CMqttMessage
 * @return none
 ** ***********************************************************************/
#ifdef USE_MQTT_HANDLER
void CLoggerHandler::addLogMessageSlot(const CMqttMessage &message)
{
    if ( m_mqttClient.isNull() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Объект MqttClient не проинициализирован!" );
    }
    else
    {
        m_mqttClient->safePublish( message );
    }
}
#endif

/** ****************************************************************************
 * @fn addLogMessage    [thread safe]
 * @brief слот для отправки LOG сообщений в соответствующий MQTT топик
 * @param CMqttMessage
 * @return none
 ** ***************************************************************************/
#ifdef USE_MQTT_HANDLER
void CLoggerHandler::addLogMessage(const CMqttMessage &message)
{
    if ( message.isValid() )
    {
        if ( ! QMetaObject::invokeMethod( this,
                                          "addLogMessageSlot",
                                          Qt::QueuedConnection,
                                          Q_ARG( CMqttMessage, message ) ) )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( "Невозможно сформировать invokeMethod!" );
        }
    }
}

#endif
/** ****************************************************************************
 * @fn getInstance [static]
 * @brief метод для создания singleton объекта класса
 * @param none
 * @return CLoggerHandler *
 ** ***************************************************************************/
#ifdef USE_MQTT_HANDLER
CLoggerHandler * CLoggerHandler::getInstance( CMqttClientHandler * mqttClient )
{
    if ( handler == nullptr )
    {
        QMutexLocker locker( & instanceMutex );
        if ( handler == nullptr )
        {
            handler = new CLoggerHandler( mqttClient );
        }
    }
    Q_ASSERT( handler != nullptr );
    return handler;
}
#endif

CLoggerHandler * CLoggerHandler::getInstance()
{
    if ( handler == nullptr )
    {
        QMutexLocker locker( & instanceMutex );
        if ( handler == nullptr )
        {
            handler = new CLoggerHandler(  );
        }
    }
    Q_ASSERT( handler != nullptr );
    return handler;
}
