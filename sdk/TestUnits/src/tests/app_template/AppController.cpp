#include "AppController.hpp"

AppController::AppController( QObject *parent ):
    CAbstractController(parent)
{
}

AppController::~AppController()
{
}

void AppController::mqttConnected()
{
}

void AppController::OnMqttMessageReceived( const QString & topic,
                      const QJsonObject & json )
{
    Q_UNUSED(topic);
    Q_UNUSED(json);
}

bool AppController::getDefaultSettings(QList<CSettingsEntry> &list)
{
    Q_UNUSED(list);

    return false; // do not override
}

void AppController::loadSettings(QList < CSettingsEntry > &list)
{
    Q_UNUSED(list);
}

void AppController::onAttach()
{
}

void AppController::onDetach()
{
}

