#pragma once

// Qt
#include <QUuid>

#include <Vehicle/VehicleSpec.h>
#include <CMainHandler/CCoreApplication.h>
#include <Mqtt/Topics/Informator/InformatorDefines.h>

// Брокер для удаленного соединения
#include "Mqtt.h"

class InformatorHandler : public CCoreApplication
{

    Q_OBJECT

public:
    InformatorHandler(QObject* parent = nullptr);
    virtual ~InformatorHandler() override;

private:
    void informatorInit();
    void subscribeOnTopics();
    void makeConnections();
    bool readInformatorSettings(InformatorSettings_t &settings);
    void loadRoutesFromFile(const QString& filePath);
    void onRoutesListChanged(const QVector<RouteInfo_t> &routes);
    void onRouteStateChanged(const RouteState_t &state);

private:
    virtual int onAttach() override;
    virtual int onDetach() override;
};
