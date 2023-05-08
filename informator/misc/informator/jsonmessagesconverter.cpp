// Qt
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QVariant>

// STD
#include <cmath>

// Local
#include "jsonmessagesconverter.h"

// ************************ конвертеры JSON для группы "informator" ***********

QByteArray JsonMessagesConverter::informatorRouteStateToJson(const RouteState_t &state, const QVector<RouteInfo_t> &routes)
{
    QJsonObject json;
     // Формируем объект json
    QString routeNumber = "";
    qint64 routeId      = -1;
    qint64 tripId       = -1;

    if(state.routeIndex >=0 && state.routeIndex < routes.size())
    {
        routeNumber = routes[state.routeIndex].routeNumber;
        if (state.routeIndex != -1) routeId = routes[state.routeIndex].idRoute;
        if (state.subrouteIndex != -1) tripId = routes[state.routeIndex].subroutes[state.subrouteIndex].idTrip;
    }

    json.insert("Route_id", QJsonValue(routeId));
    json.insert("Trip_id", QJsonValue(tripId));
    json.insert("Route_num", QJsonValue(routeNumber));
    json.insert("Current_route", QJsonValue(state.routeIndex));
    json.insert("Current_subroute", QJsonValue(static_cast<int>(state.subrouteIndex)));
    json.insert("Current_station", QJsonValue(static_cast<int>(state.stationIndex)));
    json.insert("Next_station", QJsonValue(static_cast<int>(state.nextStationIndex)));
    json.insert("User_current_station", QJsonValue(static_cast<int>(state.userStationIndex)));
    json.insert("User_next_station", QJsonValue(static_cast<int>(state.userNextStationIndex)));
    json.insert("Subroute_percentage", QJsonValue(static_cast<double>(state.subroutePercentage)));
    json.insert("Is_locate_on_station", QJsonValue(state.isLocatedOnStation));

    QJsonArray subroutesArr;
    for(const SubRouteStateInfo_t& subRoute: state.subRoutes)
    {
        QJsonObject subrouteObj;
        subrouteObj.insert("Subroute_name", QJsonValue(subRoute.subrouteName));
        QJsonArray stations;
        for(int i = 0; i<subRoute.stations.size(); i++) {
            QJsonObject obj;
            const auto &station = subRoute.stations[i];
            //obj.insert("Station_num", QJsonValue(station.statioIndex));
            obj.insert("Station_name", QJsonValue(station.mainInfo.stationName));
            int64_t arive_time = station.arriveTime.toMSecsSinceEpoch();
            obj.insert("Station_arrive_time", QJsonValue(QString::number(arive_time)));
            obj.insert("Station_duration", QJsonValue(static_cast<int>(station.stationDuration)));
            obj.insert("Station_lat", QJsonValue(station.mainInfo.lat));
            obj.insert("Station_lon", QJsonValue(station.mainInfo.lon));
            obj.insert("Station_type", QJsonValue(static_cast<int>(station.mainInfo.stationType)));
            obj.insert("Additional_info", QJsonValue(station.mainInfo.idStation));
            stations.append(QJsonValue(obj));
        }
        subrouteObj.insert("Stations", QJsonValue(stations));
        subroutesArr.append(QJsonValue(subrouteObj));
    }

    json.insert("Subroutes", QJsonValue(subroutesArr));
    QJsonDocument doc(json);
    return doc.toJson();
}

int32_t JsonMessagesConverter::findRouteIndex(const QString &routeNumber, const QVector<RouteInfo_t> &routes)
{
    for(int i=0; i<routes.size(); i++)
    {
        if(routeNumber == routes[i].routeNumber) {
            return i;
        }
    }
    return -1;
}

QByteArray JsonMessagesConverter::informatorRoutesToJson(const QVector<RouteInfo_t> &routes)
{
    QJsonArray arr;
    for(int32_t i=0; i<routes.size(); i++)
    {
        QJsonObject route;
        route.insert("Num", QJsonValue(routes[i].routeNumber));
        route.insert("Name", QJsonValue(routes[i].routeName));

        QJsonArray subArr;
        for(const SubrouteInfo_t& subRoute :  routes[i].subroutes)
        {
            subArr.append(subRoute.subrouteName);
        }
        route.insert("Subroutes", subArr);
        arr.append(QJsonValue(route));
    }

     // Сформировали объект json из одного элемента
    QJsonObject json;
    json.insert("Routes", QJsonValue(arr));
    QJsonDocument doc(json);
    return doc.toJson();
}

int32_t JsonMessagesConverter::guiJsonToCurRouteIndex(const QByteArray &json, const QVector<RouteInfo_t> &routes, int32_t *subrouteIndex)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonValue val = *doc.object().find("Route_num");
    int32_t index = findRouteIndex(val.toString(), routes);
    val = *doc.object().find("Subroute_num");
    *subrouteIndex = val.toInt();
    return index;
}

