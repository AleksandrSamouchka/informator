#ifndef JSONMESSAGESCONVERTER_H
#define JSONMESSAGESCONVERTER_H

// Qt
#include <QObject>
#include <QByteArray>
#include <QString>

// SDK
#include <CMqttTopics/CTopicValueEntry.h>
#include "Mqtt/Topics/Informator/InformatorDefines.h"

class JsonMessagesConverter
{
public:
    // Информатор.
    JsonMessagesConverter() = delete;
    static QByteArray informatorWorkModeToJson(InformatorWorkMode_t mode);   // InformatorWorkMode_t
    static QByteArray informatorRouteStateToJson(const RouteState_t &state, const QVector<RouteInfo_t> &routes);
    static QByteArray informatorRoutesToJson(const QVector<RouteInfo_t> &routes);

    // Gui
    static int32_t guiJsonToCurRouteIndex(const QByteArray &json, 
                                            const QVector<RouteInfo_t> &routes,
                                            int32_t *subrouteIndex); // Текущий маршрут, получаемый от гуя.
private:
    static int32_t findRouteIndex(const QString &routeNumber, const QVector<RouteInfo_t> &routes);
};

#endif // JSONMESSAGESCONVERTER_H
