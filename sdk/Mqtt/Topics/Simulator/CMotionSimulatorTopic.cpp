//Local
#include "CMotionSimulatorTopic.h"

#include <QString>

namespace
{
    const QString MOTION_SIMULATOR_TOPIC = "SIMULATOR/MOTION_SIMULATOR";
    const QString ACTION = "Action";
    const QString MOTION_DELAY = "MotionDelay";
    const QString ON_STATION_DLAY = "OnStationDelay";
}

CMotionSimulatorTopic::CMotionSimulatorTopic()
    : CMqttTopic(MOTION_SIMULATOR_TOPIC),
      m_action(Action::Stop),
      m_motionDelay(1),
      m_onStationDelay(4)
{}


const QByteArray CMotionSimulatorTopic::prepareData() const
{
    QJsonObject object;

    object.insert(ACTION,           m_action);
    object.insert(MOTION_DELAY,     m_motionDelay);
    object.insert(ON_STATION_DLAY,  m_onStationDelay);

    return QJsonDocument(object).toJson();
}

bool CMotionSimulatorTopic::parseData(const QByteArray &data)
{
    bool result = false;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(data).toUtf8());

    if (!jsonDoc.isNull())
    {
        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj.contains(ACTION))  m_action = Action(jsonObj[ACTION].toInt());

        if (jsonObj.contains(MOTION_DELAY))  m_motionDelay = jsonObj[MOTION_DELAY].toInt();

        if (jsonObj.contains(ON_STATION_DLAY))  m_motionDelay = jsonObj[MOTION_DELAY].toInt();

        result = true;
    }

    return setValidState(result);
}


void CMotionSimulatorTopic::setAction(Action action)
{
    m_action = action;
}

void CMotionSimulatorTopic::setMotionDelay(const int sec)
{
    m_motionDelay = sec;
}

void CMotionSimulatorTopic::setOnStationDelay(const int sec)
{
    m_onStationDelay = sec;
}
