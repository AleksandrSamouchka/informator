#ifndef CDATABASEHANDLER_H
#define CDATABASEHANDLER_H

#include "CDatabaseConnectionSettings.h"
#include <QtCore/QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtCore/QMutex>

class QTimer;

/** ***********************************************************************
 * @class CDatabaseHandler
 * @brief Создает подключение к БД с указанным именем
 * (создание неповторяющихся имен подключений - на совести разработчика),
 * выполняет реконнект к БД (при потери связи с СУБД или ее блокировки).
 * Может использоваться самостоятельно - имеется метод формирования запроса getQuery,
 * а затем где-то вне уже ведется работа с данным запросом. Интерфейса для
 * создания транзакций не предусмотрено (имеется протектный метод getDb) - наследуйтесь, реализуйте.
 *
 * !!! Слот threadStartedSlot должен быть обязательно выполнен после создания объекта
 * В данном методе создаются внутренние объекты, без этого вызова объект работать не будет
 ** ***********************************************************************/
class CDatabaseHandler : public QObject
{
    Q_OBJECT

    QSqlDatabase m_database;
    QTimer * m_reconnectTimer;
    mutable QMutex m_activeDbMutex;
    QString m_connectionName;
    QTimer * m_keepaliveTimer;

protected:
    QSqlDatabase & getDb( );

private slots:
    void reconnectTimerSlot( );
    void keepaliveEventSlot();

signals:
/*! Сигнал, который вызывает сам класс или его наследники
 *      Означает что произошли ошибки с БД и необходимо выполнить переподключение */
    void invalidDb( );
/*! Приватный сигнало только данного класса */
    void ready( QPrivateSignal );
/*! Сигналы об отключении и подключении к БД */
    void connected( QPrivateSignal );
    void disconnected( QPrivateSignal );

public slots:
    void threadStartedSlot();
    void threadStopedSlot();

    void resetKeepaliveTimer();     /*!< Qt::QueuedConnected */

public:
    CDatabaseHandler( const QString &connectionName, QObject * parent = nullptr );
    virtual ~CDatabaseHandler() {
        ;
    }
    QSqlQuery getQuery() const;
    bool isDbActive() const;

    static CDatabaseHandler * create( QObject * parent );
};

#endif // CDATABASEHANDLER_H
