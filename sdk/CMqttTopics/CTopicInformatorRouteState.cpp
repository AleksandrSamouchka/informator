#include "CTopicInformatorRouteState.h"

namespace
{
    const char *FIELD_ROUTENUM = "Route_num";                 /*!< string */
    const char *FIELD_CURRENT_ROUTE = "Current_route";
    const char *FIELD_CURRENTSUBROUTE = "Current_subroute";   /*!< int - Subroute_num */
    const char *FIELD_USERCURRENTSTATION = "User_current_station";     /*!< int - Station_num */
    const char *FIELD_CURRENTSTATION = "Current_station";     /*!< int - Station_num */
    const char *FIELD_NEXT_STATION = "Next_station";
    const char *FIELD_USER_NEXT_STATION = "User_next_station";
    const char *FIELD_SUBROUTE_PERCENTAGE = "Subroute_percentage";
    const char *FIELD_IS_LOCATE_ON_STATION = "Is_locate_on_station";
    const char *FIELD_SUBROUTES = "Subroutes";

    const char *FIELD_SUBROUTENAME = "Subroute_name";         /*!< string */
    const char *FIELD_STATIONS = "Stations";

    const char *FIELD_STATIONADDITIONALINFO = "Additional_info"; /*!< string */
    const char *FIELD_STATIONNAME = "Station_name";           /*!< string */
    const char *FIELD_STATIONARRIVETIME = "Station_arrive_time";  /*!< int64 */
    const char *FIELD_STATIONDURATION = "Station_duration";   /*!< int64 */
    const char *FIELD_STATIONLAT = "Station_lat";             /*!<  double*/
    const char *FIELD_STATIONLON = "Station_lon";             /*!< double */
    const char *FIELD_STATIONTYPE = "Station_type";           /*!< Start / Intermediate / End */

    const QStringList FIELDS_SUBROUTE{
        FIELD_STATIONS,
        FIELD_SUBROUTENAME
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicInformatorRouteState::CTopicInformatorRouteState( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Informator_routeState )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
#include <QDebug>
bool CTopicInformatorRouteState::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
//qDebug() << getData();
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_ROUTENUM ) &&
             jsonMainObj.contains( FIELD_CURRENT_ROUTE ) &&
             jsonMainObj.contains( FIELD_CURRENTSUBROUTE ) &&
             jsonMainObj.contains( FIELD_CURRENTSTATION ) &&
             jsonMainObj.contains( FIELD_USERCURRENTSTATION ) &&
             jsonMainObj.contains( FIELD_SUBROUTES ) )
        {
            m_routeNum = jsonMainObj.value( FIELD_ROUTENUM ).toString();
            if( m_routeNum.isEmpty() )
            {
//                PRINT_CONSOLE_MESSAGE_WARN("Неправильный номер маршрута!");
            }

            m_currentRoute = jsonMainObj.value( FIELD_CURRENT_ROUTE ).toInt();

            m_currentSubroute = jsonMainObj.value( FIELD_CURRENTSUBROUTE ).toInt();
            if(m_currentSubroute < 0)
            {
//                PRINT_CONSOLE_MESSAGE_WARN("Неправильный номер подмаршрута!");
            }

            m_currentStation = jsonMainObj.value( FIELD_CURRENTSTATION ).toInt();
            if(m_currentStation < 0)
            {
//                PRINT_CONSOLE_MESSAGE_WARN("Неправильный номер текущей станции!");
            }

            m_userCurrentStation = jsonMainObj.value( FIELD_USERCURRENTSTATION ).toInt();
            if(m_userCurrentStation < 0)
            {
//                PRINT_CONSOLE_MESSAGE_WARN("Неправильный номер текущей станции(2)!");
            }
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
                m_isLocateOnStation = jsonMainObj.value( FIELD_IS_LOCATE_ON_STATION ).toInt();
            }

            const QJsonArray subroutesJsonArr( jsonMainObj.value( FIELD_SUBROUTES ).toArray() );
            if ( ! subroutesJsonArr.isEmpty() )
            {
//                PRINT_CONSOLE_MESSAGE_INFO( QString( "Список подмаршрутов для маршрута %1 пуст" )
//                                            .arg( m_routeNum ) );
            }
            int subrouteIndex{ 0 };
            for ( const auto subroute : subroutesJsonArr )
            {
                const QJsonObject subrouteJsonObj( subroute.toObject() );
                if ( subrouteJsonObj.keys() == FIELDS_SUBROUTE )
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
                                    stationJsonObj.contains( FIELD_STATIONTYPE ) &&
                                    stationJsonObj.contains( FIELD_STATIONADDITIONALINFO ))
                            {
                                const CInformatorStationInfo stationInfo(
                                        stationIndex,
                                        stationJsonObj.value( FIELD_STATIONNAME ).toString(),
                                        stationJsonObj.value( FIELD_STATIONARRIVETIME ).toVariant().toLongLong(),
                                        stationJsonObj.value( FIELD_STATIONDURATION ).toVariant().toLongLong(),
                                        stationJsonObj.value( FIELD_STATIONTYPE ).toVariant().toInt(),
                                        stationJsonObj.value( FIELD_STATIONADDITIONALINFO ).toString());
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
                    else
                    {
//                        PRINT_CONSOLE_MESSAGE_INFO( QString( "Список остановочных пунктов для подмаршрута %1[%2] пуст" )
//                                .arg( subrouteInfo.m_name )
//                                .arg( subrouteInfo.m_number ) );
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
/*        }
            else
            {
                PRINT_CONSOLE_MESSAGE_INFO( QString( "Список подмаршрутов для маршрута %1 пуст" )
                                            .arg( m_routeNum ) );
            }
            */

            res = true;
        }
        else
        {
//            PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: "
//                                        + jsonMainObj.keys().join( "," ) );
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    setValidState( res );
    return res;
}
