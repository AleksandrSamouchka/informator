#include "CTopicInformatorPlayDoorsCmd.h"

namespace
{
    const char *FIELD = "Current_station_num";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicInformatorPlayDoorsCmd::CTopicInformatorPlayDoorsCmd( const int stationNum ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Informator_playDoors ),
    m_stationNum{ stationNum }
{ }

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicInformatorPlayDoorsCmd::prepareData()
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ FIELD, QJsonValue{ m_stationNum } } };
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
