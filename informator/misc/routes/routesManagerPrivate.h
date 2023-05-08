#ifndef ROUTESMANAGERPRIVATE_H
#define ROUTESMANAGERPRIVATE_H


// Qt
#include <QObject>

//SDK
#include "CConsoleLogger/CConsoleLogger.h"
#include "Mqtt/Topics/Informator/InformatorDefines.h"
#include "Mqtt/Topics/Informator/RoutesParser.h"

class RouteManagerPrivate: public QObject
{
    Q_OBJECT

    public:
        RouteManagerPrivate(QObject* parent = nullptr)
            : QObject(parent),
              dailyParserIsReady(false),
              routesParserIsReady(false),
              ready(true),
              isCollectedById(false)
    {}

    inline QVector<RouteFullInfo_t> getRoutes()
    {
        return routes;
    }

    signals:
        void routeFullInfoReady(bool ready);

    public:
        void collectRouteFullInfo();
        bool dailyParserIsReady;
        bool routesParserIsReady;
        bool ready;
        bool isCollectedById;
        QScopedPointer<RoutesParser> routesParser;
        QVector<RouteFullInfo_t> routes;
        // структура элемента списка QPair<route_id, QVector<trip.id>>
        QList<QPair<qint64, QList<qint64>>> dailyOrder;
};

#endif // ROUTESMANAGERPRIVATE_H
