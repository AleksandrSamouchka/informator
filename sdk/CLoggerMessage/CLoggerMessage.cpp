#include "CLoggerMessage.h"
#include <CConsoleLogger/CConsoleLogger.h>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonValue>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

namespace
{
/*! Поля JSON объекта */
//    static const char *LOG_JSON_MODULE = "Module_name";
    static const char *LOG_JSON_VERSION = "Module_version" ;
    static const char *LOG_JSON_TS = "Timestamp";
    static const char *LOG_JSON_FUNCTION = "Function";
    static const char *LOG_JSON_LINE = "Line";
}


/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/

CLoggerMessage::CLoggerMessage():
    CMessage(QCoreApplication::applicationName(), "", ELogCriteria::LogCriteria_info)
{
}

CLoggerMessage::CLoggerMessage( const QString &module,
                                const QString &version,
                                qint64 ts,
                                const QString &function,
                                int line,
                                const ELogCriteria &criteria,
                                const QString &message ):
    CMessage(module, message, criteria)
{
    init(/*module, */version, ts, function, line/*, criteria, message*/);
}


CLoggerMessage::CLoggerMessage( qint64 ts,
                                const QString &function,
                                int line,
                                const ELogCriteria &criteria,
                                const QString &message): 
    CMessage(QCoreApplication::applicationName(), message, criteria)

/*:
    CLoggerMessage( function, line, criteria, message )*/
{
//    m_ts = ts;
    init(/*QCoreApplication::applicationName(), */APP_VERSION_STRING, ts, function, line/*, criteria, message*/);
}

CLoggerMessage::CLoggerMessage( const QString &function,
                                int line,
                                const ELogCriteria &criteria,
                                const QString &message ) :
    CMessage(QCoreApplication::applicationName(), message, criteria)

/*:
    m_function( function ),
    m_line{ line },
    m_criteria{ criteria },
    m_message( message )*/
{
//    static auto id2 = 
//    Q_UNUSED(id2);
    init(/*QCoreApplication::applicationName(), */APP_VERSION_STRING, QDateTime::currentMSecsSinceEpoch(), function, line/*, criteria, message*/);
}

#if 0
CLoggerMessage::CLoggerMessage( const QString &jsonStr ) : CMessage
    

 /*:
    CLoggerMessage( )*/
{
    m_module.clear();
    m_message.clear();
    m_ts = 0;
    const QJsonDocument jsonDoc( QJsonDocument::fromJson( jsonStr.toUtf8() ) );
    if ( jsonDoc.isEmpty() || ! jsonDoc.isObject() )
    {
        PRINT_CONSOLE_MESSAGE_WARN( "Не удалось опознать сообщение - пустой объект" );
    }
    else if ( jsonDoc.object().keys() != LOG_JSON_KEYS )
    {
        PRINT_CONSOLE_MESSAGE_WARN( QString( "Не удалось опознать сообщение - неверный список ключей. "
                                             "Имеются[%1], должны быть[%2]" )
                                    .arg( jsonDoc.object().keys().join( "+" ) )
                                    .arg( LOG_JSON_KEYS.join( "+" ) ) );
    }
    else
    {
        const QJsonObject jsonObj( jsonDoc.object() );
        ::init(jsonObj.value( LOG_JSON_MODULE ).toString(),
                static_cast< ELogCriteria >(jsonObj.value( LOG_JSON_CRITERIA ).toInt(),
                jsonObj.value( LOG_JSON_MESSAGE ).toString());

        init(/*jsonObj.value( LOG_JSON_MODULE ).toString(), */
             jsonObj.value( LOG_JSON_VERSION ).toString(), 
             jsonObj.value( LOG_JSON_TS ).toVariant().toULongLong(), 
             jsonObj.value( LOG_JSON_FUNCTION ).toString(), 
             jsonObj.value( LOG_JSON_LINE ).toInt(), 
             /*static_cast< ELogCriteria >( LOG_CRITERIA_LIST[jsonObj.value( LOG_JSON_CRITERIA ).toString()] ), 
             jsonObj.value( LOG_JSON_MESSAGE ).toString()*/);
    }
}

#endif

void CLoggerMessage::init(/*const QString &module,*/
              const QString &version,
              qint64 ts,
              const QString &function,
              int line/*,
              const ELogCriteria &criteria,
              const QString &message*/)
{
    qRegisterMetaType<CLoggerMessage>("CLoggerMessage");
    m_version = version;
    m_ts = ts;
    m_function = function;
    m_line = line;
}


/** ***********************************************************************
 * @fn setters
 * @brief слоты для задания свойств класса
 ** ***********************************************************************/

#if 0

void CLoggerMessage::setVersion(const QString &version)
{
    m_version = version;
}

void CLoggerMessage::setTs(qint64 ts)
{
    m_ts = ts;
}

void CLoggerMessage::setFunction(const QString &function)
{
    m_function = function;
}

void CLoggerMessage::setLine(int line)
{
    m_line = line;
}

#endif


QString CLoggerMessage::getVersion() const
{
    return m_version;
}

qint64 CLoggerMessage::getTs() const
{
    return m_ts;
}

QString CLoggerMessage::getFunction() const
{
    return m_function;
}

int CLoggerMessage::getLine() const
{
    return m_line;
}


/** ***********************************************************************
 * @fn getJsonObject
 * @brief метод для получения Json объекта с полями, заполненными свойствами
 * текущего объекта
 * @param none
 * @return QJsonObject
 ** ***********************************************************************/
QJsonObject CLoggerMessage::getJsonObject() const
{
    QJsonObject obj = CMessage::getJsonObject();
    obj.insert( LOG_JSON_VERSION, m_version );
    obj.insert( LOG_JSON_TS, QJsonValue::fromVariant( QVariant( m_ts ) ) );
    obj.insert( LOG_JSON_FUNCTION, m_function );
    obj.insert( LOG_JSON_LINE, m_line );

    return obj;
}


// Костыль
int CLoggerMessage::getIndexOfCriteria(const QString &field)
{
//    int index = -1;
    for (int i=0; i<(int)CMessage::ELogCriteria::LogCriteria_last; i++)
    {
        if(field == getCriteriaString((CMessage::ELogCriteria)i))
        {
            return i;
        }       
    }
    
    return -1;
}



