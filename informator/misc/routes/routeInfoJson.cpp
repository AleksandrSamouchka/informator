#include "routeInfoJson.h"

//Qt
#include <QDir>
#include <QVariantMap>
#include <QPointer>
#include <QFile>

//Local
#include <CLoggerHandler/CLoggerHandler.h>

namespace
{
    const QString FILE_NAME = "/currentRouteInfo.json";

    const QString ROUTE_INDEX = "routeIndex";
    const QString SUBROUTE_INDEX = "subrouteIndex";
    const QString ORDERS = "orders";

    QPointer<QFile> routeInfoFile;

    QVariantMap INFO_MAP
    {
        {ORDERS, {}},
        {ROUTE_INDEX, {}},
        {SUBROUTE_INDEX, {}}
    };
}

RouteInfoJson &RouteInfoJson::getInstance()
{
    static RouteInfoJson routeInfoJson;
    return routeInfoJson;
}

void RouteInfoJson::fillOrders(QList<qint64>& orders)
{
    QJsonObject jsonObj = openFileAndGetJsonObj();

    if (jsonObj.isEmpty())
    {
        PRINT_CONSOLE_MESSAGE_ERROR("Empty JSON Object");
        return;
    }

    PRINT_CONSOLE_MESSAGE_INFO("Fill order in JSON");

    QJsonArray newOrdersArray;

    for (int i = 0; i < orders.size(); ++i)
        newOrdersArray.append(orders[i]);

    jsonObj[ORDERS] = newOrdersArray;

    QJsonDocument newDoc = QJsonDocument::fromVariant(jsonObj.toVariantMap());

    openFileAndWrite(newDoc);
}

void RouteInfoJson::setRouteIndex(int idx)
{
    QJsonObject jsonObj = openFileAndGetJsonObj();

    if (jsonObj.isEmpty()) return;

    auto val = jsonObj.find(ROUTE_INDEX);
    if (val != jsonObj.end())
        *val = idx;

    QJsonDocument newDoc = QJsonDocument::fromVariant(jsonObj.toVariantMap());

    PRINT_CONSOLE_MESSAGE_INFO(QString("Set route index is %1").arg(idx));

    openFileAndWrite(newDoc);
}

int RouteInfoJson::getRouteIndex()
{
    QJsonObject jsonObj = openFileAndGetJsonObj();

    if (jsonObj.isEmpty()) return -1;

    auto index = jsonObj[ROUTE_INDEX];

    PRINT_CONSOLE_MESSAGE_INFO(QString("Get route index is %1").arg(index.toInt()));

    return index.toInt();
}

void RouteInfoJson::setSubrouteIndex(int idx)
{
    QJsonObject jsonObj = openFileAndGetJsonObj();

    if (jsonObj.isEmpty()) return;

    auto val = jsonObj.find(SUBROUTE_INDEX);
    if (val != jsonObj.end())
        *val = idx;

    PRINT_CONSOLE_MESSAGE_INFO(QString("Set new subroute index is %1").arg(idx));

    QJsonDocument newDoc = QJsonDocument::fromVariant(jsonObj.toVariantMap());

    openFileAndWrite(newDoc);
}

int RouteInfoJson::getSubrouteIndex()
{
    QJsonObject jsonObj = openFileAndGetJsonObj();

    if (jsonObj.isEmpty()) return -1;

    auto index = jsonObj[SUBROUTE_INDEX];

    PRINT_CONSOLE_MESSAGE_INFO(QString("Get subroute index is %1").arg(index.toInt()));

    return index.toInt();
}

RouteInfoJson::RouteInfoJson()
{
    routeInfoFile = new QFile(QDir::currentPath() + FILE_NAME);

    if (!routeInfoFile->exists())
    {
        QJsonDocument doc = QJsonDocument::fromVariant(INFO_MAP);

        openFileAndWrite(doc);
    }
}

QJsonObject RouteInfoJson::openFileAndGetJsonObj()
{
    routeInfoFile->open(QIODevice::ReadOnly);

    if (routeInfoFile->isOpen())
        PRINT_CONSOLE_MESSAGE_INFO("File open succesfully");
    else
        PRINT_CONSOLE_MESSAGE_ERROR("Cannot open file");

    QByteArray rawData = routeInfoFile->readAll();

    routeInfoFile->close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData);

    if (!jsonDoc.isObject())
    {
        PRINT_CONSOLE_MESSAGE_WARN("routeInfoData is not object");
        return QJsonDocument::fromVariant(INFO_MAP).object();
    }

    QJsonObject jsonObj = jsonDoc.object();

    return jsonDoc.object();
}

void RouteInfoJson::openFileAndWrite(const QJsonDocument &doc)
{
    routeInfoFile->open(QIODevice::WriteOnly | QIODevice::Truncate);

    if (routeInfoFile->isOpen())
        PRINT_CONSOLE_MESSAGE_INFO("File open succesfully");
    else
        PRINT_CONSOLE_MESSAGE_ERROR("Cannot open file");

    routeInfoFile->write(doc.toJson());
    routeInfoFile->flush();
    routeInfoFile->close();
}

