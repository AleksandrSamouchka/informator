#include "CTopicIntegrationInformator.h"

static const char *FIELD_CURRENT_STATION = "Current_station";
static const char *FIELD_NEXT_STATION = "Next_station";
static const char *FIELD_ADDITIONAL_INF = "Additional_inf";
static const char *FIELD_ON_STATION = "On_station";
static const char *FIELD_ROUTE_NUM = "Route_num";
static const char *FIELD_ROUTE_NAME = "Route_name";
static const char *FIELD_SUBROUTE_NAME = "Subroute_name";

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param const CTopicValueEntry
 * @return bool
 ** ***********************************************************************/
bool CTopicIntegrationInformator::initFromData(const CTopicValueEntry & value)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_CURRENT_STATION )
             && jsonMainObj.contains( FIELD_NEXT_STATION )
             && jsonMainObj.contains( FIELD_ADDITIONAL_INF )
             && jsonMainObj.contains( FIELD_ON_STATION )
             && jsonMainObj.contains( FIELD_ROUTE_NUM )
             && jsonMainObj.contains( FIELD_ROUTE_NAME )
             && jsonMainObj.contains( FIELD_SUBROUTE_NAME ) )
        {
            currentStation = jsonMainObj.value( FIELD_CURRENT_STATION ).toString();
            nextStation = jsonMainObj.value( FIELD_NEXT_STATION ).toString();
            additionalInf = jsonMainObj.value( FIELD_ADDITIONAL_INF ).toString();
            onStation = jsonMainObj.value( FIELD_ON_STATION ).toBool();
            routeNum = jsonMainObj.value( FIELD_ROUTE_NUM ).toString();
            routeName = jsonMainObj.value( FIELD_ROUTE_NAME ).toString();
            subrouteName = jsonMainObj.value( FIELD_SUBROUTE_NAME ).toString();
            res = true;
        }
        else
        {
            printUnknownFieldsListMessage( jsonMainObj.keys() );
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    setValidState( res );
    return res;
}

/** ***********************************************************************
* @fn prepareData
* @brief метод формирования значения топика из полей структуры
* @param none
* @return QByteArray
** ***********************************************************************/
QByteArray CTopicIntegrationInformator::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_CURRENT_STATION, currentStation );
    jsonMainObj.insert( FIELD_NEXT_STATION, nextStation );
    jsonMainObj.insert( FIELD_ADDITIONAL_INF, additionalInf );
    jsonMainObj.insert( FIELD_ON_STATION, onStation );
    jsonMainObj.insert( FIELD_ROUTE_NUM, routeNum );
    jsonMainObj.insert( FIELD_ROUTE_NAME, routeName );
    jsonMainObj.insert( FIELD_SUBROUTE_NAME, subrouteName );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
