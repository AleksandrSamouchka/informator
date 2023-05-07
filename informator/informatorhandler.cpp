#include "informatorhandler.h"
#include "misc/cinformatormisc.h"

InformatorHandler::InformatorHandler(QObject* parent) :
    CCoreApplication(parent)
{
}

InformatorHandler::~InformatorHandler()
{}

int InformatorHandler::onAttach()
{

    VehicleSpec::init(this);
    makeConnections();
    informatorInit();
    subscribeOnTopics();

    return 0;
}

int InformatorHandler::onDetach()
{
    return 0;
}

void InformatorHandler::informatorInit()
{
    InformatorSettings_t settings;
    readInformatorSettings(settings);
}

void InformatorHandler::subscribeOnTopics()
{

}

void InformatorHandler::makeConnections()
{

}

bool InformatorHandler::readInformatorSettings(InformatorSettings_t &settings)
{
    bool ok;
    InformatorSettings_t *defaultSettings = CInformatorMisc::defaultSettings();
    settings.watchdogTimeout = CInformatorMisc::readValueAndReport("InformatorWatchdogTimeout", defaultSettings->watchdogTimeout, QVariant::Int).toInt(&ok);
    settings.stationRadius = CInformatorMisc::readValueAndReport("InformatorStationRadius", defaultSettings->stationRadius, QVariant::Double).toFloat(&ok);
    settings.stationHisteresis = CInformatorMisc::readValueAndReport("InformatorStationHisteresis", defaultSettings->stationHisteresis, QVariant::Double).toFloat(&ok);

    return true;
}

void InformatorHandler::loadRoutesFromFile(const QString& filePath)
{

}

void InformatorHandler::onRoutesListChanged(const QVector<RouteInfo_t> &routes)
{

}

void InformatorHandler::onRouteStateChanged(const RouteState_t &state)
{

}
