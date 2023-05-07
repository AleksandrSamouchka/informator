#include <QDebug>

#include "CDatabaseHandler.h"
#include <CLoggerHandler/CLoggerHandler.h>
#include <CConsoleLogger/CConsoleLogger.h>

#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

namespace
{
/*! database name */
    const char *DB_NAME = "mntconti_database";
/*! refresh database interval (reconnect if DB is unconnected and so on) */
    constexpr int DB_REFRESH_INTERVAL = 10 * 1000;                         /*!< 10 sec in ms */
    constexpr int DB_KEEPALIVE_INTERVAL = 10 * 60 * 1000;                  /*!< 10 min in ms */

    const char *WARN_STRING_INACTIVE = "Подключение к БД не активно! Запрос проигнорирован";
}

/** ***********************************************************************
 * @brief Класс по работе с БД проекта
 ** ***********************************************************************/
CDatabaseHandler::CDatabaseHandler(const QString &connectionName, QObject * parent) :
    QObject( parent ),
    m_connectionName( connectionName )
{
    if(QSqlDatabase::contains(m_connectionName)) 
    {
        m_database = QSqlDatabase::database( m_connectionName, false );
    }
    else 
    {
        m_database = QSqlDatabase::addDatabase( "QMYSQL", m_connectionName );
    }
}

/** ***********************************************************************
 * @fn getDb
 * @brief протектный метод, для получения объекта подключения к БД (для наследуемых классов)
 * @param none
 * @return none
 ** ***********************************************************************/
QSqlDatabase &CDatabaseHandler::getDb()
{
    return m_database;
}

/** ***********************************************************************
 * @fn isDbActive
 * @brief протектный метод, проверяющий состояние обработчика (коннект к БД)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CDatabaseHandler::isDbActive() const
{
    bool res{ false };
    if ( m_activeDbMutex.tryLock() )
    {
        m_activeDbMutex.unlock();
        res = true;
    }
    return res;
}

/** ***********************************************************************
 * @fn create
 * @brief создает новое подключение к БД
 * @param parent Родитель нового объекта
 * @return none
 ** ***********************************************************************/

CDatabaseHandler * CDatabaseHandler::create( QObject * parent )
{
    Q_ASSERT( parent != nullptr );
    return new CDatabaseHandler{ parent->metaObject()->className(), parent };
}


/** ***********************************************************************
 * @fn reconnectTimerSlot
 * @brief слот, выполняет переподключение к БД
 * @param none
 * @return none
 ** ***********************************************************************/
void CDatabaseHandler::reconnectTimerSlot()
{
    bool res{ true };
    if ( m_database.isOpen() )
    {
        m_database.close();
        emit disconnected( QPrivateSignal{} );
    }
/*! 1. Проверка настроек */
    CDatabaseConnectionSettings dbSettings{};
    if ( ! dbSettings.isValid() )
    {
        PRINT_CONSOLE_MESSAGE_ERROR( QString("Некорректные настройки подключения к серверу БД MySQL [%1] - проверьте настройки!").arg( dbSettings.getLastError() ) );
        res = false;
    }
/*! 2. Проверяем наличие подключения к серверу СУБД */
    if ( res )
    {
        m_database.setHostName( dbSettings.getIp() );
        m_database.setPort( dbSettings.getPort() );
        m_database.setDatabaseName( DB_NAME );
        m_database.setUserName( dbSettings.getUser() );
        m_database.setPassword( dbSettings.getPassword() );
        if ( ! m_database.open() )
        {
            PRINT_CONSOLE_MESSAGE_ERROR( QString("Невозможно подключиться к серверу БД MySQL[%1] - проверьте настройки!").arg( m_database.lastError().databaseText() ) );
            res = false;
        }
        else
        {
//            PRINT_CONSOLE_MESSAGE_INFO( "Выполнено успешное подключение к БД MySQL" );
            QSqlQuery query( m_database );
            query.exec("SET character_set_client = 'utf8'");
            query.exec("SET character_set_results = 'utf8'");
            query.exec("SET character_set_connection = 'utf8'");
            emit connected( QPrivateSignal{} );
        }
    }
    if ( res )
    {
        m_reconnectTimer->stop();
        m_activeDbMutex.unlock();
    }
    else
    {
        m_reconnectTimer->start();
    }
}

/** ***********************************************************************
 * @fn keepaliveEventSlot
 * @brief слот, выполнение быстрого запроса для поддержания коннекта с БД
 * @param none
 * @return none
 ** ***********************************************************************/
void CDatabaseHandler::keepaliveEventSlot()
{
    if ( ! isDbActive() )
    {
        PRINT_LOG_MESSAGE_WARN( WARN_STRING_INACTIVE );
        PRINT_CONSOLE_MESSAGE_WARN( WARN_STRING_INACTIVE );
    }
    else
    {
        QSqlQuery query( m_database );
        if ( ! query.exec( "SELECT CURRENT_TIMESTAMP;" ) )
        {
            const QString message{ QString( "Невозможно выполнить SQL запрос[%1], ошибка[%2]" )
                        .arg( "SELECT CURRENT_TIMESTAMP;" )
                        .arg( query.lastError().text() ) };
            PRINT_LOG_MESSAGE_ERROR( message );
            PRINT_CONSOLE_MESSAGE_ERROR( message );
            emit invalidDb();
        }
    }
}

/** ***********************************************************************
 * @fn threadStart
 * @brief слот, вызывается после переноса объекта в нужный поток и его запуска.
 * Если объект не будет переноситься в отдельный поток - данный слот запускать
 * сразу
 * @param none
 * @return none
 ** ***********************************************************************/
void CDatabaseHandler::threadStartedSlot()
{
/* таймер переподключения */
    m_reconnectTimer = new QTimer( this );
    m_reconnectTimer->setSingleShot( false );
    m_reconnectTimer->setInterval( DB_REFRESH_INTERVAL );
    connect( m_reconnectTimer, &QTimer::timeout, this, &CDatabaseHandler::reconnectTimerSlot, Qt::QueuedConnection );
    connect(this, &CDatabaseHandler::invalidDb, this, [this]()
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Обнаружен сбой в работе с сервером MySQL - выполняется переподключение!" );
        if ( m_activeDbMutex.tryLock() )
        {
            m_reconnectTimer->start();
        }
    }, Qt::QueuedConnection );

    m_activeDbMutex.tryLock();
    reconnectTimerSlot();

/* таймер поддержания коннекта с БД */
    m_keepaliveTimer = new QTimer( this );
    m_keepaliveTimer->setSingleShot( false );
    m_keepaliveTimer->setInterval( DB_KEEPALIVE_INTERVAL );
    connect( m_keepaliveTimer, &QTimer::timeout, this, &CDatabaseHandler::keepaliveEventSlot, Qt::QueuedConnection );
    m_keepaliveTimer->start();

    emit ready( QPrivateSignal{} );
}

/** ***********************************************************************
 * @fn threadStart
 * @brief слот, вызывается перед удалением объекта - остановка всех
 * необходимых процессов
 * @param none
 * @return none
 ** ***********************************************************************/
void CDatabaseHandler::threadStopedSlot()
{
    m_reconnectTimer->stop();
    m_keepaliveTimer->stop();
    m_database.close();
}

/** ***********************************************************************
 * @fn resetKeepaliveTimer
 * @brief слот, сбрасывает таймер поддержания коннекта с БД.
 * Сбрасывать следует каждый выполненный запрос в БД. Действие совсем не
 * обязательное, запрос активности очень быстрый
 * @param none
 * @return none
 ** ***********************************************************************/
void CDatabaseHandler::resetKeepaliveTimer()
{
    m_keepaliveTimer->start();
}

/** ***********************************************************************
 * @fn getQuery
 * @brief метод получения запроса (для заполнения) в БД
 * @param none
 * @return QSqlQuery
 ** ***********************************************************************/
QSqlQuery CDatabaseHandler::getQuery() const
{
    Q_ASSERT( thread() == QThread::currentThread() );
    return QSqlQuery( m_database );
}
