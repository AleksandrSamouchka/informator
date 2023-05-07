#ifndef CTOPICSTOPPANEL_H
#define CTOPICSTOPPANEL_H

#include <QString>

// SDK
#include <Mqtt/CMqttTopic.h>

class CTopicStopPanel : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
    public:
        CTopicStopPanel()
            : CMqttTopic()
        {}
#endif
    public:
        enum STOP_PANEL_ACTIONS
        {
            UNKNOWN,
            BLINK_STOP,
            PASSANGER,
            THANKS
        };

        explicit CTopicStopPanel();
        STOP_PANEL_ACTIONS action() const;
        void setAction(STOP_PANEL_ACTIONS act);

    protected:
        const QByteArray prepareData() const override;
        bool parseData(const QByteArray& data) override;

    private:
        QString actionToString(STOP_PANEL_ACTIONS act) const ;
        STOP_PANEL_ACTIONS stringToAction(const QString& act) const ;

    private:
        STOP_PANEL_ACTIONS m_action;
};

#endif // CTOPICSTOPPANEL_H
