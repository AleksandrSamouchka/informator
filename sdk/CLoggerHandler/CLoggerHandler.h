#ifndef CLOGGERHANDLER_H
#define CLOGGERHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#ifdef USE_MQTT_HANDLER
    #include <CMqttHandler/CMqttClientHandler.h>
    #include <CMqttHandler/CMqttMessage.h>
#endif

#include <CLoggerMessage/CLoggerMessage.h>
#include <CLoggerMessage/CGuiMessage.h>
#include <CConsoleLogger/CConsoleLogger.h>

#include <QScopedPointer>
#include <QPointer>
#include <QMutex>

class QThread;

//#ifdef USE_MQTT_HANDLER
//class CMqttClientHandler;
//#endif

/*! Макросы для формирования структуры LOG сообщения и его публикации в топике.
 *      Для простого и удобного использования.
 *      Наименование функции, номер строки и время сообщения формируется автоматически.
 */
/*#ifdef USE_MQTT_HANDLER
    #define PRINT_LOG_MESSAGE( message, criteria, retain ) CLoggerHandler::getInstance()->addLogMessage( \
    CMqttMessage( CLoggerHandler::getLoggerTopicName(), \
    CREATE_LOG_MESSAGE( message, criteria ).getJsonString(), \
    retain ) )

    #define PRINT_LOG_MESSAGE_INFO( message ) PRINT_LOG_MESSAGE(message, CLoggerMessage::ELogCriteria::LogCriteria_info, false ), PRINT_CONSOLE_MESSAGE_INFO( message )
    #define PRINT_LOG_MESSAGE_WARN( message ) PRINT_LOG_MESSAGE(message, CLoggerMessage::ELogCriteria::LogCriteria_warning, false ), PRINT_CONSOLE_MESSAGE_WARN( message )
    #define PRINT_LOG_MESSAGE_ERROR( message ) PRINT_LOG_MESSAGE(message, CLoggerMessage::ELogCriteria::LogCriteria_error, false ), PRINT_CONSOLE_MESSAGE_ERROR( message )

    #define PRINT_LOG_RETAIN_MESSAGE_INFO( message ) PRINT_LOG_MESSAGE( message, CLoggerMessage::ELogCriteria::LogCriteria_info, true ), PRINT_CONSOLE_MESSAGE_INFO( message )
    #define PRINT_LOG_RETAIN_MESSAGE_WARN( message ) PRINT_LOG_MESSAGE( message, CLoggerMessage::ELogCriteria::LogCriteria_warning, true ), PRINT_CONSOLE_MESSAGE_WARN( message )
    #define PRINT_LOG_RETAIN_MESSAGE_ERROR( message ) PRINT_LOG_MESSAGE( message, CLoggerMessage::ELogCriteria::LogCriteria_error, true ), PRINT_CONSOLE_MESSAGE_ERROR( message )

#else
*/
//    #define PRINT_LOG_MESSAGE( message, criteria, retain ) while(0) {}

    #define PRINT_LOG_MESSAGE_INFO( message )   PRINT_CONSOLE_MESSAGE_INFO( message )
    #define PRINT_LOG_MESSAGE_WARN( message )   PRINT_CONSOLE_MESSAGE_WARN( message )
    #define PRINT_LOG_MESSAGE_ERROR( message )  PRINT_CONSOLE_MESSAGE_ERROR( message )

    #define PRINT_LOG_RETAIN_MESSAGE_INFO( message ) PRINT_CONSOLE_MESSAGE_INFO( message )
    #define PRINT_LOG_RETAIN_MESSAGE_WARN( message ) PRINT_CONSOLE_MESSAGE_WARN( message )
    #define PRINT_LOG_RETAIN_MESSAGE_ERROR( message ) PRINT_CONSOLE_MESSAGE_ERROR( message )

//#endif


/*! Макросы для формирования структуры сообщения, которое будет отображено на GUI.
 *      Для простого и удобного использования.
 */
#ifdef USE_MQTT_HANDLER
    #define PRINT_GUI_MESSAGE( name, message, criteria, retain ) CLoggerHandler::getInstance()->addLogMessage( \
    CMqttMessage( CLoggerHandler::getGuiMessageTopicName(), \
    CREATE_GUI_MESSAGE( name, message, criteria ).getJsonString(), \
    /*0,*/ \
    retain ) )
#else
    #define PRINT_GUI_MESSAGE( name, message, criteria, retain ) while(0) {}
#endif
#define PRINT_GUI_MESSAGE_INFO( name, message ) PRINT_GUI_MESSAGE( name, message, CLoggerMessage::ELogCriteria::LogCriteria_info, false )
#define PRINT_GUI_MESSAGE_WARN( name, message ) PRINT_GUI_MESSAGE( name, message, CLoggerMessage::ELogCriteria::LogCriteria_warning, false )
#define PRINT_GUI_MESSAGE_ERROR( name, message ) PRINT_GUI_MESSAGE( name, message, CLoggerMessage::ELogCriteria::LogCriteria_error, false )

#define PRINT_GUI_RETAIN_MESSAGE_INFO( name, message ) PRINT_GUI_MESSAGE( name, message, CLoggerMessage::ELogCriteria::LogCriteria_info, true )
#define PRINT_GUI_RETAIN_MESSAGE_WARN( name, message ) PRINT_GUI_MESSAGE( name, message, CLoggerMessage::ELogCriteria::LogCriteria_warning, true )
#define PRINT_GUI_RETAIN_MESSAGE_ERROR( name, message ) PRINT_GUI_MESSAGE( name, message, CLoggerMessage::ELogCriteria::LogCriteria_error, true )

/** ***********************************************************************
 * @class CLoggerHandler
 * @brief Класс одиночка, публикует ЛОГ сообщения в соответствующем топике
 ** ***********************************************************************/
class CLoggerHandler : public QObject
{
    Q_OBJECT
protected:
#ifdef USE_MQTT_HANDLER
    explicit CLoggerHandler( CMqttClientHandler * mqttClient, QObject *parent = nullptr );
#endif
    explicit CLoggerHandler(  QObject *parent = nullptr );
    virtual ~CLoggerHandler() {
        ;
    }
protected slots:
//    void threadStartedSlot();
//    void threadStopedSlot();
//    void deinitSlot();
signals:
    void ready( QPrivateSignal );

/* инструменты для обеспечения потокобезопасного доступа к публичным методам */
protected slots:
#ifdef USE_MQTT_HANDLER
    void addLogMessageSlot( const CMqttMessage &message );
#endif
/* интерфейс */
public slots:
#ifdef USE_MQTT_HANDLER
    void addLogMessage( const CMqttMessage &message );
#endif

public:
#ifdef USE_MQTT_HANDLER
    static CLoggerHandler * getInstance(CMqttClientHandler * mqttClient);
#endif
    static CLoggerHandler * getInstance();
    static void deinitLoggerHandler( );
    static QString getLoggerTopicName( );
    static QString getGuiMessageTopicName( );
private:
#ifdef USE_MQTT_HANDLER
    QPointer < CMqttClientHandler > m_mqttClient;
//    QQueue<CMqttMessage> qeMessages;
#endif
//    bool mqttClientInited{false};
    static QPointer<CLoggerHandler> handler;
//    static QScopedPointer<QThread> handlerThread;
    static const int LOG_MESSAGE_QUEUE_SIZE;
    static const int CREATE_MQTT_CLIENT_TIMEOUT;
    //static bool handlerInited;
    static QMutex initHandlerMutex;
    static QMutex instanceMutex;

};

#endif // CLOGGERHANDLER_H
