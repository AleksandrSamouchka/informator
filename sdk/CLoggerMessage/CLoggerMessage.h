#ifndef CLOGGERMESSAGE_H
#define CLOGGERMESSAGE_H

#include <QtCore/QCoreApplication>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>
#include <CUtils/CUtils.h>

#include "CMessage.h"


    #define APP_VERSION_STRING QCoreApplication::applicationVersion().split('.').mid(0, 3).join('.')
    #define SDK_VERSION_STRING  QString(CUtils::sdkVersion()).split('.').mid(0, 3).join('.')

/*! Макросы для формирования структуры LOG сообщения.
 *      Наименование функции, номер строки и время сообщения формируется автоматически.
 */
    #define CREATE_LOG_MESSAGE( message, criteria ) CLoggerMessage( Q_FUNC_INFO, __LINE__, criteria, message )
    #define CREATE_LOG_MESSAGE_INFO( message ) CLoggerMessage( Q_FUNC_INFO, __LINE__, CLoggerMessage::ELogCriteria::LogCriteria_info, message )
    #define CREATE_LOG_MESSAGE_WARN( message ) CLoggerMessage( Q_FUNC_INFO, __LINE__, CLoggerMessage::ELogCriteria::LogCriteria_warning, message )
    #define CREATE_LOG_MESSAGE_ERROR( message ) CLoggerMessage( Q_FUNC_INFO, __LINE__, CLoggerMessage::ELogCriteria::LogCriteria_error, message )

/** ***********************************************************************
 * @class CLoggerMessage
 * @brief Класс, описывает необходимые поля сообщения в ЛОГ
 ** ***********************************************************************/
class CLoggerMessage :  public CMessage
{
private:
    QString m_version;
    qint64 m_ts;
    QString m_function;
    int m_line;

public:
    CLoggerMessage();  
  
    explicit CLoggerMessage( const QString &module,
                             const QString &version,
                             qint64 ts,
                             const QString &function,
                             int line,
                             const CMessage::ELogCriteria &criteria,
                             const QString &message );

    explicit CLoggerMessage( qint64 ts,
                             const QString &function,
                             int line,
                             const CMessage::ELogCriteria &criteria,
                             const QString &message );

    explicit CLoggerMessage( const QString &function,
                             int line,
                             const CMessage::ELogCriteria &criteria/* = ELogCriteria::LogCriteria_info*/,
                             const QString &message/* = QString{} */);

    virtual ~CLoggerMessage() {}


    QString getVersion() const;
    qint64 getTs() const;
    QString getFunction() const;
    int getLine() const;
    virtual QJsonObject getJsonObject() const;

    // Костыль
    static int getIndexOfCriteria(const QString &field);

protected:
    void init(/*const QString &module,*/
              const QString &version,
              qint64 ts,
              const QString &function,
              int line/*,
              const ELogCriteria &criteria,
              const QString &message*/);
};

#endif // CLOGGERMESSAGE_H
