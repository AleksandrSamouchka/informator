#pragma once

#include <QObject>

#include "Mqtt/Topics/Informator/InformatorDefines.h"
#include "Mqtt/Topics/Informator/RoutesParser.h"

class RouteManager : public QObject
{
    Q_OBJECT
public:
    explicit RouteManager(QObject *parent = nullptr);
signals:
    void routesReady(QVector<RouteFullInfo_t>);
public slots:
    void onRouteTopic(const QByteArray& data);
private:
    void collectRoutes(const QByteArray& data);
};

