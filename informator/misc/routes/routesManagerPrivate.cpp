// Local
#include "routesManagerPrivate.h"


void RouteManagerPrivate::collectRouteFullInfo()
{
    ready = false;
    routes.clear();

    for (const auto& el : dailyOrder)
    {
        qint64 routeId = el.first;
        RouteFullInfo_t routeFullInfo;

        routeFullInfo.idDepot = routesParser->idDepot();
        routeFullInfo.idAgency = routesParser->agency();
        routeFullInfo.depotName = routesParser->depotName();

        QString routeName = routesParser->title(routeId);
        routeFullInfo.title.routeNumber = routeName;
        routeFullInfo.title.routeName = routeName;
        routeFullInfo.title.routeFullName = routeName;
        routeFullInfo.title.idRoute = routeId;
        routeFullInfo.title.resources = routesParser->routeResources(routeId);
        routeFullInfo.title.ledPanelSettings = routesParser->ledPanelSettings(routeId);

        // If in daily_order trip_id is empty, we grap all subroutes by routeID
        // And give it to informator
        if (el.second[0] == -1) // If first id empty, then all id empty
        {
            PRINT_CONSOLE_MESSAGE_INFO("In daily order not set tripID, get subroutes by routeID");

            const auto& subroutes = routesParser->subroutes(routeId);
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
                    qDebug() << QString("У routeID %1 нет станций").arg(routeId);
                    continue;
                }
                else
                {
                    firstStation = routeFullInfo.title.subroutes[i].stations.first().stationName;
                    endStation = routeFullInfo.title.subroutes[i].stations.last().stationName;
                }
                routeFullInfo.title.subroutes[i].subrouteName += (": " + nameDirection + firstStation + " - " + endStation);
            }
            isCollectedById = false;
        }
        else
        {
            PRINT_CONSOLE_MESSAGE_INFO("Collect routes by tripID from daily orders");
            const auto& subroutesSize = el.second.size();
            routeFullInfo.title.subroutes.resize(subroutesSize);

            for (int i = 0; i < subroutesSize; ++i)
            {
                qint64 tripId = el.second[i];
                bool searchResault = false;
                routeFullInfo.title.subroutes[i] = routesParser->subroute(routeId, tripId, searchResault);

                if (searchResault)
                {
                    auto direction = routeFullInfo.title.subroutes[i].tripType;
                    const QString& nameDirection = direction == TripDirection::Reverse ? "Обр. " : "Прям. ";
                    routeFullInfo.title.subroutes[i].subrouteName = routeFullInfo.title.subroutes[i].shortDescription;
                    const QString& firstStation = routeFullInfo.title.subroutes[i].stations[0].stationName;
                    const QString& endStation = routeFullInfo.title.subroutes[i].stations[routeFullInfo.title.subroutes[i].stations.size() - 1].stationName;
                    routeFullInfo.title.subroutes[i].subrouteName += (": " + nameDirection + firstStation + " - " + endStation);
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_ERROR("CAN'T FIND SUBROUTE BY DAILY ID");
                }
            }

            isCollectedById = true;
        }

        routes.append(routeFullInfo);
    }

    // So, we decide, that parser received and parse info
    // We set handlers flag to false, because there is posiibility,
    // That they can received another info
    dailyParserIsReady = false;
    routesParserIsReady = false;

    PRINT_CONSOLE_MESSAGE_INFO("Collect route and push it to informator");

    // And set flag ready, we collect data to informator
    ready = true;
    emit routeFullInfoReady(ready);
}
