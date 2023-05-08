#pragma once

// Qt
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>

/*!
 * \brief The RouteInfoJson class - singleton class
 *  Class for handle current route info, save it, and update
 */
class RouteInfoJson
{

public:
    static RouteInfoJson& getInstance();
    static void    fillOrders(QList<qint64> &orders);    
    static void    setRouteIndex(int idx);
    static int     getRouteIndex();
    static void    setSubrouteIndex(int idx);
    static int     getSubrouteIndex();
private:
    explicit RouteInfoJson();
    RouteInfoJson(const RouteInfoJson& other) = delete;
    RouteInfoJson& operator = (const RouteInfoJson& other) = delete;
    static QJsonObject openFileAndGetJsonObj();
    static void openFileAndWrite(const QJsonDocument& doc);
};


