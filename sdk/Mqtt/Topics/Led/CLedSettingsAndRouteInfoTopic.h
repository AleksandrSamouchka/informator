#ifndef CLEDSETTINGSANDROUTEINFOTOPIC_H
#define CLEDSETTINGSANDROUTEINFOTOPIC_H

// SDK
#include <Mqtt/CMqttTopic.h>
#include <Mqtt/Topics/Informator/RouteNetworkStructures.h>


class CLedSettingsAndRouteInfoTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
    public:
        CLedSettingsAndRouteInfoTopic()
            : CMqttTopic()
        {}
#endif
    public:
        explicit CLedSettingsAndRouteInfoTopic();
        QString routeNumber() const;
        TextRC_t ledNum() const;
        TextRC_t ledSide() const;
        TextRC_t ledFront() const;
        TextRC_t ledInternal() const;
        LedSettings_t LedSettings() const;

        void setRouteNumber(const QString& routeNumber);
        void setLedNum(const TextRC_t& ledNum);
        void setLedSide(const TextRC_t& ledSide);
        void setLedFront(const TextRC_t& ledFront);
        void setLedInternal(const TextRC_t& ledInternal);
        void setLedSettings(const LedSettings_t& settings);

    protected:
        const QByteArray prepareData() const override;
        bool parseData(const QByteArray& data) override;

    private:      
       void fillLedSettings(const QJsonObject& obj, LedSettings_t& m_ledSettings);
       void ledSettingsToJson(QJsonObject& obj, const LedSettings_t& m_ledSettings) const;

    private:
        QString m_routeNumber;
        TextRC_t m_ledNum;
        TextRC_t m_ledSide;
        TextRC_t m_ledFront;
        TextRC_t m_ledInternal;
        LedSettings_t m_ledSettings;
};

#endif // CLEDSETTINGSANDROUTEINFOTOPIC_H
