#include "CInformatorRouteStateTopic.h"

namespace
{
    const QString SUBTOPICNAME                 = "ROUTE_STATE";

    const QString FIELD_ROUTEID                = "Route_id";
    const QString FIELD_ROUTENUM               = "Route_num";                 /*!< string */
    const QString FIELD_TRIPID                 = "Trip_id";
    const QString FIELD_CURRENT_ROUTE          = "Current_route";
    const QString FIELD_CURRENTSUBROUTE        = "Current_subroute";   /*!< int - Subroute_num */
    const QString FIELD_USERCURRENTSTATION     = "User_current_station";     /*!< int - Station_num */
    const QString FIELD_CURRENTSTATION         = "Current_station";     /*!< int - Station_num */
    const QString FIELD_NEXT_STATION           = "Next_station";
    const QString FIELD_USER_NEXT_STATION      = "User_next_station";
    const QString FIELD_SUBROUTE_PERCENTAGE    = "Subroute_percentage";
    const QString FIELD_IS_LOCATE_ON_STATION   = "Is_locate_on_station";
    const QString FIELD_SUBROUTES              = "Subroutes";

    const QString FIELD_SUBROUTENAME           = "Subroute_name";         /*!< string */
    const QString FIELD_STATIONS               = "Stations";

    const QString FIELD_STATIONNAME            = "Station_name";           /*!< string */
    const QString FIELD_STATIONARRIVETIME      = "Station_arrive_time";  /*!< int64 */
    const QString FIELD_STATIONDURATION        = "Station_duration";   /*!< int64 */
    const QString FIELD_STATIONLAT             = "Station_lat";             /*!<  double*/
    const QString FIELD_STATIONLON             = "Station_lon";             /*!< double */
    const QString FIELD_STATIONTYPE            = "Station_type";           /*!< Start / Intermediate / End */
    const QString FIELD_ADDITIONALINFO         = "Additional_info";    /*!< string */
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CInformatorRouteStateTopic::CInformatorRouteStateTopic() :
    CGrInformatorTopic( SUBTOPICNAME )
{ }

CInformatorRouteStateTopic::CInformatorRouteStateTopic(const QByteArray & data) :
    CGrInformatorTopic( SUBTOPICNAME, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CInformatorRouteStateTopic::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_ROUTENUM, "00" },
                               { FIELD_CURRENTSUBROUTE, 1 },
                               { FIELD_USERCURRENTSTATION, 2 },
                               { FIELD_CURRENTSTATION, 3 },
                               { FIELD_NEXT_STATION, 4 },
                               { FIELD_USER_NEXT_STATION, 5 },
                               { FIELD_SUBROUTE_PERCENTAGE, 6 },
                               { FIELD_IS_LOCATE_ON_STATION, true } } );

    QJsonArray subroutesJsonArr{};
    QJsonObject subrouteJsonObj( { { FIELD_SUBROUTENAME, "testSubrouteName" } } );
    QJsonArray stationsJsonArr{};

    QJsonObject stationJsonObj( { { FIELD_STATIONNAME, "testStationName" },
                                  { FIELD_STATIONARRIVETIME, 7 },
                                  { FIELD_STATIONDURATION, 8 },
                                  { FIELD_STATIONLAT, 9.0 },
                                  { FIELD_STATIONLON, 10.0 },
                                  { FIELD_STATIONTYPE, 0 },
                                  { FIELD_ADDITIONALINFO, "additionalInfo" } } );
    stationsJsonArr.append( stationJsonObj );
    stationJsonObj = {{ FIELD_STATIONNAME, "testStationName2" },
                      { FIELD_STATIONARRIVETIME, 7 },
                      { FIELD_STATIONDURATION, 8 },
                      { FIELD_STATIONLAT, 9.0 },
                      { FIELD_STATIONLON, 10.0 },
                      { FIELD_STATIONTYPE, 0 },
                      { FIELD_ADDITIONALINFO, "additionalInfo" }};
    stationsJsonArr.append( stationJsonObj );

    subrouteJsonObj.insert( FIELD_STATIONS, stationsJsonArr );
    subroutesJsonArr.append(subrouteJsonObj);
    jsonMainObj.insert( FIELD_SUBROUTES, subroutesJsonArr );

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CInformatorRouteStateTopic::setRouteNum(const QString &value)
{
    m_routeNum = value;
}

void CInformatorRouteStateTopic::setCurrentRoute(const int value)
{
    m_currentRoute = value;
}

void CInformatorRouteStateTopic::setRouteId(const qint64 value)
{
    m_routeId = value;
}

void CInformatorRouteStateTopic::setTripId(const qint64 value)
{
    m_tripId = value;
}

void CInformatorRouteStateTopic::setCurrentSubroute(const int value)
{
    m_currentSubroute = value;
}

void CInformatorRouteStateTopic::setUserCurrentStation(const int value)
{
    m_userCurrentStation = value;
}

void CInformatorRouteStateTopic::setCurrentStation(const int value)
{
    m_currentStation = value;
}

void CInformatorRouteStateTopic::setNextStation(const int value)
{
    m_nextStation = value;
}

void CInformatorRouteStateTopic::setUserNextStation(const int value)
{
    m_userNextStation = value;
}

void CInformatorRouteStateTopic::setSubroutePercentage(const int value)
{
    m_subroutePercentage = value;
}

void CInformatorRouteStateTopic::setIsLocateOnStation(const bool value)
{
    m_isLocateOnStation = value;
}

void CInformatorRouteStateTopic::clearSubroutes()
{
    m_subroutes.clear();
}

void CInformatorRouteStateTopic::appendSubroute(const CInformatorRouteStateTopic::CInformatorSubrouteInfo &value)
{
    m_subroutes.append( value );
}

#ifdef MNT_TOPICS_TEST
QByteArray CInformatorRouteStateTopic::getEtalonJson() const
{
    QJsonObject jsonMainObj( { { FIELD_ROUTENUM, "00" },
                               { FIELD_CURRENTSUBROUTE, 1 },
                               { FIELD_USERCURRENTSTATION, 2 },
                               { FIELD_CURRENTSTATION, 3 },
                               { FIELD_NEXT_STATION, 4 },
                               { FIELD_USER_NEXT_STATION, 5 },
                               { FIELD_SUBROUTE_PERCENTAGE, 6 },
                               { FIELD_IS_LOCATE_ON_STATION, true } } );

    QJsonArray subroutesJsonArr{};
    QJsonObject subrouteJsonObj( { { FIELD_SUBROUTENAME, "testSubrouteName" } } );
    QJsonArray stationsJsonArr{};
        QJsonObject stationJsonObj( { { FIELD_STATIONNAME, "testStationName" },
                                      { FIELD_STATIONARRIVETIME, 7 },
                                      { FIELD_STATIONDURATION, 8 },
                                      { FIELD_STATIONLAT, 9.0 },
                                      { FIELD_STATIONLON, 10.0 },
                                      { FIELD_STATIONTYPE, 0 } } );
        stationsJsonArr.append( stationJsonObj );
    subrouteJsonObj.insert( FIELD_STATIONS, stationsJsonArr );
    jsonMainObj.insert( FIELD_SUBROUTES, subroutesJsonArr );

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

/** ***********************************************************************
 * @brief prepareData
 ** ***********************************************************************/
const QByteArray CInformatorRouteStateTopic::prepareData() const 
{
    QJsonObject jsonMainObj( { { FIELD_ROUTEID, m_routeId },
                               { FIELD_ROUTENUM, m_routeNum },
                               { FIELD_TRIPID, m_tripId },
                               { FIELD_CURRENT_ROUTE, m_currentRoute },
                               { FIELD_CURRENTSUBROUTE, m_currentSubroute },
                               { FIELD_USERCURRENTSTATION, m_userCurrentStation },
                               { FIELD_CURRENTSTATION, m_currentStation },
                               { FIELD_NEXT_STATION, m_nextStation },
                               { FIELD_USER_NEXT_STATION, m_userNextStation },
                               { FIELD_SUBROUTE_PERCENTAGE, m_subroutePercentage },
                               { FIELD_IS_LOCATE_ON_STATION, m_isLocateOnStation } } );
    QJsonArray subroutesJsonArr{};
    for ( const auto & i : m_subroutes )
    {
        QJsonObject subrouteJsonObj( { { FIELD_SUBROUTENAME, i.m_name } } );
        QJsonArray stationsJsonArr{};
        for ( const auto & j : i.m_stations )
        {
            QJsonObject stationJsonObj( { { FIELD_STATIONNAME, j.m_name },
                                          { FIELD_STATIONARRIVETIME, j.m_arriveTime },
                                          { FIELD_STATIONDURATION, j.m_duration },
                                          { FIELD_STATIONLAT, j.m_latitude },
                                          { FIELD_STATIONLON, j.m_longitude },
                                          { FIELD_STATIONTYPE, j.m_type },
                                          { FIELD_ADDITIONALINFO, j.m_additionalInfo } } );
            stationsJsonArr.append( stationJsonObj );
        }
        subrouteJsonObj.insert( FIELD_STATIONS, stationsJsonArr );
        subroutesJsonArr.append(subrouteJsonObj);
    }

    jsonMainObj.insert( FIELD_SUBROUTES, subroutesJsonArr );
//    QByteArray jsonData{
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
//};
//    return setData( /*jsonData*/jsonMainObj );
}

/** ***********************************************************************
 * @brief parseData
 ** ***********************************************************************/
bool CInformatorRouteStateTopic::parseData(const QByteArray & data)
{
    m_subroutes.clear();
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ROUTEID ) &&
             jsonMainObj.contains( FIELD_TRIPID ) &&
             jsonMainObj.contains( FIELD_ROUTENUM ) &&
             jsonMainObj.contains( FIELD_CURRENT_ROUTE ) &&
             jsonMainObj.contains( FIELD_CURRENTSUBROUTE ) &&
             jsonMainObj.contains( FIELD_CURRENTSTATION ) &&
             jsonMainObj.contains( FIELD_USERCURRENTSTATION ) &&
             jsonMainObj.contains( FIELD_SUBROUTES ) )
        {
            m_routeId = jsonMainObj.value( FIELD_ROUTEID ).toInt();
            m_tripId = jsonMainObj.value(FIELD_TRIPID).toInt();
            m_routeNum = jsonMainObj.value( FIELD_ROUTENUM ).toString();
            m_currentRoute = jsonMainObj.value( FIELD_CURRENT_ROUTE ).toInt();
            m_currentSubroute = jsonMainObj.value( FIELD_CURRENTSUBROUTE ).toInt();
            m_currentStation = jsonMainObj.value( FIELD_CURRENTSTATION ).toInt();
            m_userCurrentStation = jsonMainObj.value( FIELD_USERCURRENTSTATION ).toInt();
            if ( jsonMainObj.contains( FIELD_NEXT_STATION ) )
            {
                m_nextStation = jsonMainObj.value( FIELD_NEXT_STATION ).toInt();
            }
            if ( jsonMainObj.contains( FIELD_USER_NEXT_STATION ) )
            {
                m_userNextStation = jsonMainObj.value( FIELD_USER_NEXT_STATION ).toInt();
            }
            if ( jsonMainObj.contains( FIELD_SUBROUTE_PERCENTAGE ) )
            {
                m_subroutePercentage = jsonMainObj.value( FIELD_SUBROUTE_PERCENTAGE ).toInt();
            }
            if ( jsonMainObj.contains( FIELD_IS_LOCATE_ON_STATION ) )
            {
                m_isLocateOnStation = jsonMainObj.value( FIELD_IS_LOCATE_ON_STATION ).toBool();
            }

            const QJsonArray subroutesJsonArr( jsonMainObj.value( FIELD_SUBROUTES ).toArray() );
            int subrouteIndex{ 0 };
            for ( const auto subroute : subroutesJsonArr )
            {
                const QJsonObject subrouteJsonObj( subroute.toObject() );
                if ( subrouteJsonObj.contains( FIELD_STATIONS ) &&
                     subrouteJsonObj.contains( FIELD_SUBROUTENAME ) )
                {
                    CInformatorSubrouteInfo subrouteInfo( subrouteIndex,
                            subrouteJsonObj.value( FIELD_SUBROUTENAME ).toString() );
                    const QJsonArray stationsJsonArr( subrouteJsonObj.value( FIELD_STATIONS ).toArray() );
                    if ( ! stationsJsonArr.isEmpty() )
                    {
                        int stationIndex{ 0 };
                        for ( const auto station : stationsJsonArr )
                        {
                            const QJsonObject stationJsonObj( station.toObject() );
                            if ( stationJsonObj.contains( FIELD_STATIONARRIVETIME ) &&
                                    stationJsonObj.contains( FIELD_STATIONDURATION ) &&
                                    stationJsonObj.contains( FIELD_STATIONLAT ) &&
                                    stationJsonObj.contains( FIELD_STATIONLON ) &&
                                    stationJsonObj.contains( FIELD_STATIONNAME ) &&
                                    stationJsonObj.contains( FIELD_STATIONTYPE ) )
                            {
                                const CInformatorStationInfo stationInfo(
                                        stationIndex,
                                        stationJsonObj.value( FIELD_STATIONNAME ).toString(),
                                        stationJsonObj.value( FIELD_STATIONARRIVETIME ).toVariant().toLongLong(),
                                        stationJsonObj.value( FIELD_STATIONDURATION ).toVariant().toLongLong(),
                                        stationJsonObj.value( FIELD_STATIONLAT ).toVariant().toDouble(),
                                        stationJsonObj.value( FIELD_STATIONLON ).toVariant().toDouble(),
                                        stationJsonObj.value( FIELD_STATIONTYPE ).toVariant().toInt(),
                                        stationJsonObj.value( FIELD_ADDITIONALINFO ).toString() );
                                subrouteInfo.m_stations.append( stationInfo );
                            }
                            else
                            {
                                PRINT_CONSOLE_MESSAGE_WARN( "JSON объект STATION содержит неизвестный список полей: "
                                        + stationJsonObj.keys().join( "," ) );
                            }
                            stationIndex ++;
                        }
                    }
                    m_subroutes.append( subrouteInfo );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "JSON объект SUBROUTE содержит неизвестный список полей: "
                            + subrouteJsonObj.keys().join( "," ) );
                }
                subrouteIndex ++;
            }
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
