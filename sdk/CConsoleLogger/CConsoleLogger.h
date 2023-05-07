#ifndef CCONSOLELOGGER_H
#define CCONSOLELOGGER_H

#include <CLoggerMessage/CLoggerMessage.h>
#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QTextCodec>
#include <QScopedPointer>


typedef enum
{
    LF_SHORT = 0,
    LF_LONG
} LogFormat;

/*! Макросы для вывода сообщения в консоль (используется цветная подсветка) */
    #define PRINT_CONSOLE_MESSAGE_INFO( message ) CConsoleLogger::printMessage( CLoggerMessage( Q_FUNC_INFO, __LINE__, CLoggerMessage::ELogCriteria::LogCriteria_info, message ) )
    #define PRINT_CONSOLE_MESSAGE_WARN( message ) CConsoleLogger::printMessage( CLoggerMessage( Q_FUNC_INFO, __LINE__, CLoggerMessage::ELogCriteria::LogCriteria_warning, message ) )
    #define PRINT_CONSOLE_MESSAGE_ERROR( message ) CConsoleLogger::printMessage( CLoggerMessage( Q_FUNC_INFO, __LINE__, CLoggerMessage::ELogCriteria::LogCriteria_error, message ) )

class CConsoleLogger;

/** ***********************************************************************
 * @class ConsoleLoggerCreator
 * @brief Класс для создания в куче экземпляра класса CConsoleLogger в
 * главном потоке. Ради того, чтобы именно в куче и именно в главном потоке
 * класс и создавался
 ** ***********************************************************************/
/*
class ConsoleLoggerCreator
{
public:
    ConsoleLoggerCreator();
    CConsoleLogger *getConsoleLoggerInstance();
private:
    QScopedPointer<CConsoleLogger> consoleLogger;
};
*/


/** ***********************************************************************
 * @class CConsoleLogger
 * @brief Класс, потокораздельно выводит сообщения в консоль
 ** ***********************************************************************/
class CConsoleLogger : public QObject
{
    Q_OBJECT
//    friend class ConsoleLoggerCreator;
protected:
    explicit CConsoleLogger( QObject *parent = nullptr );

protected slots:
    void printMessageSlot(const CLoggerMessage &message);

public:
    static CConsoleLogger * getInstance();
    virtual ~CConsoleLogger();
    static void deinitLoggerHandler( );
    static void printMessage(const CLoggerMessage &message);
    static bool isVerbose();
    static bool isQuiet();
    static void setQuiet(const bool on);
    static void setVerbose(const bool on);
    static void setFormat(const LogFormat &format);
private:
//    static ConsoleLoggerCreator m_loggerCreator;
    QAtomicInt m_verbose;
    QAtomicInt m_quiet;
    QAtomicInt m_format;
};

#endif // CCONSOLELOGGER_H
