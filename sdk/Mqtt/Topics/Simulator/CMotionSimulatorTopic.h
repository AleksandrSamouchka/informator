#pragma once

//Local
#include <Mqtt/CMqttTopic.h>

//Qt
#include <QHash>

enum Action
{
    Stop = 0,
    Start,
    Pause
};


class CMotionSimulatorTopic : public CMqttTopic
{
public:
    explicit CMotionSimulatorTopic();
    void setAction(Action action);
    void setMotionDelay(const int sec);
    void setOnStationDelay(const int sec);
    inline Action getAction() const
    {
        return m_action;
    }
    inline int getMotionDelay() const
    {
        return m_motionDelay;
    }
    inline int getOnStationDelay() const
    {
        return m_onStationDelay;
    }
protected:
    const QByteArray prepareData() const override;
    bool parseData(const QByteArray &data) override;
private:
    Action m_action;
    /*!
     * \brief m_motionDelay - задержка в секундах между отправкой
     * координат движения автобуса
     */
    int m_motionDelay;
    /*!
     * \brief m_onStationDelay - Задержка между отправкой действий на остановке
     * Координаты остановки - Задержка - Открыть двери - Задержка - Закрыть двери
     */
    int m_onStationDelay;

};

