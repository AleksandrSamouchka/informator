#include "CGuiMessage.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonValue>

namespace
{
/*! Поля JSON объекта */
    static const char *LOG_JSON_NAME = "Message_name";
}

/** ***********************************************************************
 * @fn CGuiMessage  [конструктор]
 ** ***********************************************************************/
CGuiMessage::CGuiMessage() : 
    CMessage(QCoreApplication::applicationName(), "", CLoggerMessage::ELogCriteria::LogCriteria_info)
{
}
CGuiMessage::CGuiMessage( const QString &module,
                          const QString &name,
                          const QString &message,
                          const CLoggerMessage::ELogCriteria &criteria ): 
    CMessage(module, message, criteria)
{ 
    m_name = name;
}

CGuiMessage::CGuiMessage( const QString &name,
                          const QString &message,
                          const CLoggerMessage::ELogCriteria &criteria ):
    CMessage(QCoreApplication::applicationName(), message, criteria)

{ 
    m_name = name;
}



/** ***********************************************************************
 * @fn getters
 * @brief методы для чтения свойств класса
 ** ***********************************************************************/

QString CGuiMessage::getName() const
{
    return m_name;
}


/** ***********************************************************************
 * @fn getJsonString
 * @brief метод для получения JSON объекта в виде строки
 * @param none
 * @return QString
 ** ***********************************************************************/
QString CGuiMessage::getJsonString() const
{
    QJsonObject obj = CMessage::getJsonObject();
    obj.insert( LOG_JSON_NAME, m_name );
    return QJsonDocument( obj ).toJson( QJsonDocument::Compact );
}

