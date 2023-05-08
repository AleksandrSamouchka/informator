#include "routeManager.h"

#include <QFuture>
#include <QtConcurrent/QtConcurrent>

RouteManager::RouteManager(QObject *parent) : QObject(parent)
{}

void RouteManager::onRouteTopic(const QByteArray &data)
{
    collectRoutes(data);
}

void RouteManager::collectRoutes(const QByteArray &data)
{
    QFuture<void> collect = QtConcurrent::run([=]()
    {
        RoutesParser parser(data);

        if (parser.isNull())
        {
            qWarning() << "Не удалось распарсить маршрутную сеть";
            return;
        }

        QVector<RouteFullInfo_t> result;
        QList<qint64> routesId = parser.routesID();

        for (const auto& id : qAsConst(routesId))
        {
            RouteFullInfo_t routeFullInfo;

            routeFullInfo.idDepot = parser.idDepot();
            routeFullInfo.idAgency = parser.agency();
            routeFullInfo.depotName = parser.depotName();

            QString routeName = parser.title(id);
            routeFullInfo.title.routeNumber = routeName;
            routeFullInfo.title.routeName = routeName;
            routeFullInfo.title.routeFullName = routeName;
            routeFullInfo.title.idRoute = id;
            routeFullInfo.title.resources = parser.routeResources(id);
            routeFullInfo.title.ledPanelSettings = parser.ledPanelSettings(id);

            const auto& subroutes = parser.subroutes(id);
            const auto& subroutesSize = subroutes.size();
            routeFullInfo.title.subroutes.resize(subroutesSize);

            for (int i = 0; i < subroutesSize; ++i)
            {
                routeFullInfo.title.subroutes[i] = subroutes[i];
                auto direction = routeFullInfo.title.subroutes[i].tripType;
                const QString& nameDirection = direction == TripDirection::Reverse ? "Обр. " : "Прям. ";
                routeFullInfo.title.subroutes[i].subrouteName = routeFullInfo.title.subroutes[i].shortDescription;

                /*Проверяем, а есть ли вообще станции*/
                QString firstStation{""};
                QString endStation{""};

                if (routeFullInfo.title.subroutes[i].stations.isEmpty())
                {
                    qDebug() << QString("У routeID %1 нет станций").arg(id);
                    continue;
                }
                else
                {
                    firstStation = routeFullInfo.title.subroutes[i].stations.first().stationName;
                    endStation = routeFullInfo.title.subroutes[i].stations.last().stationName;
                }
                routeFullInfo.title.subroutes[i].subrouteName += (": " + nameDirection + firstStation + " - " + endStation);
            }

            result.append(routeFullInfo);
        }

        emit routesReady(result);
    });
}
