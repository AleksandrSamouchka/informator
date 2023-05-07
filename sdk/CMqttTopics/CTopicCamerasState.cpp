#include "CTopicCamerasState.h"

namespace
{
    const QString TOPIC_NAME = "CAMERA_STATE/";

    const QString IS_PRESENT_KEY = "isPresent";
    const QString IS_STREAMING_KEY = "isStreaming";
    const QString IS_RECORDING_KEY = "isRecording";
}

CTopicCamerasState::CTopicCamerasState(const QString &cameraName)
    : CMqttTopic(TOPIC_NAME + cameraName)
{}


const QByteArray CTopicCamerasState::prepareData() const
{
    QJsonObject obj;
    obj.insert(IS_PRESENT_KEY, m_status.isPresent);
    obj.insert(IS_STREAMING_KEY, m_status.isStreaming);
    obj.insert(IS_RECORDING_KEY, m_status.isPresent);

    return QJsonDocument(obj).toJson();
}

bool CTopicCamerasState::parseData(const QByteArray &data)
{
    QJsonObject obj = QJsonDocument::fromJson(QString(data).toUtf8()).object();

    if (obj.contains(IS_PRESENT_KEY) && obj.contains(IS_STREAMING_KEY)
            && obj.contains(IS_RECORDING_KEY))
    {
        m_status.isPresent = obj[IS_PRESENT_KEY].toBool();
        m_status.isStreaming = obj[IS_STREAMING_KEY].toBool();
        m_status.isRecording = obj[IS_PRESENT_KEY].toBool();

        return setValidState(true);
    }

    return setValidState(false);
}
