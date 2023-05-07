#pragma once

#include <Mqtt/CMqttTopic.h>

struct Status
{
    bool isPresent{false};
    bool isStreaming{false};
    bool isRecording{false};
};

class CTopicCamerasState : public CMqttTopic
{
public:
    CTopicCamerasState(const QString& cameraName);
    inline void setStatus(const Status& status){m_status = status;}
    inline Status getStatus(){return m_status;}
protected:
    const QByteArray prepareData() const override;
    bool parseData(const QByteArray &data) override;
private:
    Status  m_status;
};

