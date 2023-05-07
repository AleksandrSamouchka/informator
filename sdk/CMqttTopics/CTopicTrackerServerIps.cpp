#include "CTopicTrackerServerIps.h"

namespace
{
    const char *FIELD = "Servers";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicTrackerServerIps::CTopicTrackerServerIps( const QStringList &servers ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Tracker_ServerIps ),
    m_serverIps{ servers }
{ }

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicTrackerServerIps::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD, QJsonArray::fromStringList( m_serverIps ) );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
