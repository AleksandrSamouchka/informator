#include "CConsoleLogger.h"
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <memory>

#ifndef __cplusplus
    #error Application is not use with C
#endif

#ifdef Q_OS_WIN
    #include <windows.h>
#endif
#include <iostream>

/*! Макросы для вывода сообщения в консоль (используется цветная подсветка) */
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
/* цвет */
    #define CONS_BLACK          30
    #define CONS_RED            31
    #define CONS_GREEN          32
    #define CONS_YELLOW         33
    #define CONS_BLUE           34
    #define CONS_PURPLE         35
    #define CONS_CYAN           36
    #define CONS_WHITE          37
/* макросы установки цвета консоли */
    #define COLOURF(cletter) "\033[" << cletter << "m"
    #define CHOOSE_COLOR(is_left, left, right) COLOURF( ( is_left ? left : right ) )
    #define CRESET() "\033[0m"
    #define FORMAT_STR(str) QString(str).toStdString()
/* макрос вывода сообщения в консоль */
/*
    #define PRINT_CONSOLE_MESSAGE( message ) \
    std::cout << COLOURF(CONS_YELLOW) << "[" << FORMAT_STR(message.getModule()) << " " << FORMAT_STR(message.getVersion()) << "]" \
    << COLOURF(CONS_PURPLE) << "{" << FORMAT_STR(QDateTime::fromMSecsSinceEpoch(message.getTs()).toString("dd-MM-yyyy hh:mm:ss.zzz")) << "}" \
    << CHOOSE_COLOR( message.getCriteria() == CLoggerMessage::ELogCriteria::LogCriteria_info, CONS_GREEN, CONS_RED ) \
    << " - [" << FORMAT_STR(message.getCriteriaString()) << "] " \
    << COLOURF(CONS_CYAN) << "(" << FORMAT_STR(message.getFunction()) << " - " << message.getLine() << ") " \
    << COLOURF(CONS_GREEN) << FORMAT_STR(message.getMessage()) << CRESET() << std::endl << std::flush
*/
//#endif
#elif defined(Q_OS_WIN)
/* цвет */
    #define CONS_BLACK          0
    #define CONS_RED            4
    #define CONS_GREEN          2
    #define CONS_YELLOW         14
    #define CONS_BLUE           1
    #define CONS_PURPLE         5
    #define CONS_CYAN           11
    #define CONS_WHITE          15
/* макросы установки цвета консоли */
    #define COLOURF(cletter) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),cletter)
    #define CRESET() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15)
    #define FORMAT_STR(str) QTextCodec::codecForName("IBM 866")->fromUnicode(str).data()
/* макрос вывода сообщения в консоль */
/*
    #define PRINT_CONSOLE_MESSAGE( message ) \
    COLOURF(CONS_YELLOW); std::cout << "[" << FORMAT_STR(message.getModule()) << " " << FORMAT_STR(message.getVersion()) << "]"; \
    COLOURF(CONS_PURPLE); std::cout << "{" << FORMAT_STR(QDateTime::fromMSecsSinceEpoch(message.getTs()).toString("dd-MM-yyyy hh:mm:ss.zzz")) << "}"; \
    COLOURF(CONS_RED); std::cout << "(" << FORMAT_STR(message.getFunction()) << " - " << message.getLine() << ")"; \
    COLOURF(CONS_GREEN); std::cout << " - [" << FORMAT_STR(message.getCriteriaString()) << "] " << FORMAT_STR(message.getMessage()); \
    CRESET(); std::cout << std::endl << std::flush
*/
#endif

namespace
{
    //bool handlerInited{ false };
    //QMutex initHandlerMutex{};
    //QMutex instanceMutex{};
    //CConsoleLogger * handler = nullptr;
    //QThread *handlerThread = nullptr;
    static QScopedPointer<CConsoleLogger> consoleLogger(nullptr);
}


//ConsoleLoggerCreator CConsoleLogger::m_loggerCreator;

/** ***********************************************************************
 * @fn deinitDatabaseHandler
 * @brief функция для деинициализации singleton объекта обработчика запросов в БД
 * @param none
 * @return none
 ** ***********************************************************************/
void CConsoleLogger::deinitLoggerHandler( ) // Теперь освобождение памяти делается автоматически
{
    //QMutexLocker locker( & initHandlerMutex );
    //if ( handlerInited )
    //{
    //    handlerThread->quit(); // Все удалится умным указателем само при
    //    закрытии приложения
    //    handlerThread->wait();
    //    delete handlerThread;
    //    handlerThread = nullptr;
    //    delete handler;
//        handler.reset(nullptr);
        //handler = nullptr;
    //    handlerInited = false;
    //}
}

/** ***********************************************************************
 * @fn getInstance [static]
 * @brief метод для создания singleton объекта класса
 * @param none
 * @return CConsoleLogger *
 ** ***********************************************************************/
CConsoleLogger * CConsoleLogger::getInstance( )
{
    if(consoleLogger.isNull())
    {
//        CConsoleLogger *handler = m_loggerCreator. getConsoleLoggerInstance();
        consoleLogger.reset(new CConsoleLogger());
    }

    /*if ( handler == nullptr )
    {
        QMutexLocker locker( & instanceMutex );
        if ( handler == nullptr )
        {
            handler = new CConsoleLogger( );
            handler->m_verbose.store( qstrcmp( getenv( "MNTCONTI_DEBUG" ), "1" ) == 0 ? 1 : 0 );
        }
    }*/
    Q_ASSERT( consoleLogger != nullptr );
    return consoleLogger.data();
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CConsoleLogger::CConsoleLogger(QObject *parent) : QObject(parent)
{
    //QMutexLocker locker( & initHandlerMutex );
    //handlerThread = QCoreApplication::instance()->thread(); //   new QThread( );
    //moveToThread( handlerThread );
    // handlerThread->start(); //
    m_verbose.store(0);
    m_quiet.store(0);
    m_format.store(LF_SHORT);
}

CConsoleLogger::~CConsoleLogger()
{
}


/** ***********************************************************************
 * @fn printMessageSlot
 * @brief слот для непосредственного вывода в консоль
 * @param CLoggerMessage
 * @return none
 ** ***********************************************************************/
void CConsoleLogger::printMessageSlot(const CLoggerMessage &message )
{
//    PRINT_CONSOLE_MESSAGE( message );
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    CLoggerMessage::ELogCriteria crit = message.getCriteria();
    if(getInstance()->m_format.load() == LF_LONG)
    {
        std::cout 
        << COLOURF(CONS_YELLOW) << "[" << FORMAT_STR(message.getModule()) << " " << FORMAT_STR(message.getVersion()) << "]"
        << COLOURF(CONS_PURPLE) << "{" << FORMAT_STR(QDateTime::fromMSecsSinceEpoch(message.getTs()).toString("dd-MM-yyyy hh:mm:ss.zzz")) << "} ";
//        << CHOOSE_COLOR( crit == CLoggerMessage::ELogCriteria::LogCriteria_info, CONS_GREEN, CONS_RED )
//        << " - ";
    }
    std::cout 
    << CHOOSE_COLOR( crit == CLoggerMessage::ELogCriteria::LogCriteria_info, CONS_GREEN, CONS_RED )
    << "[" << FORMAT_STR(message.getCriteriaString()) << "] " 
    << COLOURF(CONS_CYAN) << "(" << FORMAT_STR(message.getFunction()) << " - " << message.getLine() << ") " 
    << COLOURF(CONS_GREEN) << FORMAT_STR(message.getMessage()) << CRESET();
    // << std::endl << std::flush;
#elif defined(Q_OS_WIN)
    COLOURF(CONS_YELLOW); std::cout << "[" << FORMAT_STR(message.getModule()) << " " << FORMAT_STR(message.getVersion()) << "]"; 
    COLOURF(CONS_PURPLE); std::cout << "{" << FORMAT_STR(QDateTime::fromMSecsSinceEpoch(message.getTs()).toString("dd-MM-yyyy hh:mm:ss.zzz")) << "}"; 
    COLOURF(CONS_RED); std::cout << "(" << FORMAT_STR(message.getFunction()) << " - " << message.getLine() << ")"; 
    COLOURF(CONS_GREEN); std::cout << " - [" << FORMAT_STR(message.getCriteriaString()) << "] " << FORMAT_STR(message.getMessage()); 
    CRESET(); 
#endif
    std::cout << std::endl << std::flush;
}

/** ***********************************************************************
 * @fn printMessage [static]
 * @brief метод для вывода сообщения в консоль
 * @param CLoggerMessage
 * @return none
 ** ***********************************************************************/
void CConsoleLogger::printMessage(const CLoggerMessage &message )
{
    if(isQuiet() == false)
    {
        QMetaObject::invokeMethod( CConsoleLogger::getInstance(),
                               "printMessageSlot",
                               Qt::AutoConnection,
                               Q_ARG( CLoggerMessage, message ) );
    }
}

bool CConsoleLogger::isVerbose()
{
    return getInstance()->m_verbose.load() != 0;
}

bool CConsoleLogger::isQuiet()
{
    return getInstance()->m_quiet.load() != 0;
}

void CConsoleLogger::setVerbose(const bool on)
{
    getInstance()->m_verbose.store(on ? 1 : 0);
}

void CConsoleLogger::setQuiet(const bool on)
{
    getInstance()->m_quiet.store(on ? 1 : 0);
}


void CConsoleLogger::setFormat(const LogFormat &format)
{
    getInstance()->m_format.store(format);
}

/*
ConsoleLoggerCreator::ConsoleLoggerCreator()
{
    consoleLogger.reset(new CConsoleLogger);
}

CConsoleLogger *ConsoleLoggerCreator::getConsoleLoggerInstance()
{
    return consoleLogger.data();
}*/
