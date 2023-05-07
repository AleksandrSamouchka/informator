#include "CInformatorPlayStationTopic.h"

static const char *SUBTOPICNAME = "PLAY_STATION";

static const char *FIELD_STATIONNUM = "Station_num";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CInformatorPlayStationTopic::CInformatorPlayStationTopic() :
    CGrInformatorTopic( SUBTOPICNAME )
{ }

CInformatorPlayStationTopic::CInformatorPlayStationTopic(const int stationNum) :
    CGrInformatorTopic( SUBTOPICNAME ),
    m_stationNum{ stationNum }
{
    prepareData();
}

CInformatorPlayStationTopic::CInformatorPlayStationTopic(const QByteArray & data) :
    CGrInformatorTopic( SUBTOPICNAME, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CInformatorPlayStationTopic::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_STATIONNUM, 1 } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

/** ***********************************************************************
 * @brief setStationNum
 ** ***********************************************************************/
void CInformatorPlayStationTopic::setStationNum(const int value)
{
    m_stationNum = value;
}

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
const QByteArray CInformatorPlayStationTopic::prepareData() const 
{
    QJsonObject jsonMainObj( { { FIELD_STATIONNUM, m_stationNum } } );
//    QByteArray jsonData{ 
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
//};
//    return setData( /*jsonData*/jsonMainObj );
}

bool CInformatorPlayStationTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_STATIONNUM ) )
        {
            m_stationNum = jsonMainObj.value( FIELD_STATIONNUM ).toInt();
            res = true;
        }
        else
        {
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    return setValidState( res );
}
