#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>

#include "CMessage.h"

static const char *LOG_CRITERIA_LIST[ static_cast< int >( CMessage::ELogCriteria::LogCriteria_last )] =
{
        "ERROR",
        "WARN",
        "INFO"
};

static const char *LOG_JSON_MODULE = "Module_name";
static const char *LOG_JSON_CRITERIA = "Criteria";
static const char *LOG_JSON_MESSAGE = "Message";



CMessage::CMessage()
{
    init(QCoreApplication::applicationName(), "", ELogCriteria::LogCriteria_info);
}

CMessage::CMessage( const QString &module,
                 const QString &message,
                 const ELogCriteria &criteria )
{
    init(module,message, criteria);
}

CMessage::~CMessage()
{
}

void CMessage::init(const QString &module,
                 const QString &message,
                 const ELogCriteria &criteria)
{
    m_module = module;
    m_criteria = criteria;
    m_message = message;
}

QString CMessage::getModule() const
{
    return m_module;
}

CMessage::ELogCriteria CMessage::getCriteria() const
{
    return m_criteria;
}

const char *CMessage::getCriteriaString() const
{
    return getCriteriaString(m_criteria);
}

const char *CMessage::getCriteriaString(ELogCriteria index)
{
    if(index >= CMessage::ELogCriteria::LogCriteria_last)
    {
        return "UNCK";
    }

    return LOG_CRITERIA_LIST[(int)index]; 
}

QString CMessage::getMessage() const
{
    return m_message;
}

#if 0
void CMessage::setModule(const QString &module)
{
    m_module = module;
}

void CMessage::setCriteria(const CMessage::ELogCriteria &criteria)
{
    m_criteria = criteria;
}

void CMessage::setMessage(const QString &message)
{
    m_message = message;
}
#endif

QJsonObject CMessage::getJsonObject() const
{
    QJsonObject obj{};
    obj.insert( LOG_JSON_MODULE, m_module );
//    obj.insert( LOG_JSON_VERSION, m_version );
//    obj.insert( LOG_JSON_TS, QJsonValue::fromVariant( QVariant( m_ts ) ) );
//    obj.insert( LOG_JSON_FUNCTION, m_function );
//    obj.insert( LOG_JSON_LINE, m_line );
    obj.insert( LOG_JSON_CRITERIA, getCriteriaString() );
    obj.insert( LOG_JSON_MESSAGE, m_message );
    return obj;
}


QString CMessage::getJsonString() const
{
//    const QString jsonStr( QString::fromUtf8( QJsonDocument( getJsonObject() ).toJson( QJsonDocument::Com
//    return jsonStr;
    return QJsonDocument( getJsonObject() ).toJson( QJsonDocument::Compact );
}
