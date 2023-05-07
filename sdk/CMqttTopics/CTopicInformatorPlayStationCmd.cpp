#include "CTopicInformatorPlayStationCmd.h"

namespace
{
    const char *FIELD_STATIONNUM = "Station_num";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicInformatorPlayStationCmd::CTopicInformatorPlayStationCmd( int stationNum ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Informator_playStop ),
    m_stationNum{ stationNum }
{ }

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicInformatorPlayStationCmd::prepareData()
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ FIELD_STATIONNUM, QJsonValue( m_stationNum ) } };
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
