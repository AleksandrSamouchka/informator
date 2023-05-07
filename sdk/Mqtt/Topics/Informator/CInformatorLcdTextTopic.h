#ifndef CINFORMATORLCDTEXTTOPIC_H
#define CINFORMATORLCDTEXTTOPIC_H

// SDK
#include <Mqtt/CMqttTopic.h>
#include <Mqtt/Topics/Informator/RouteNetworkStructures.h>

class CInformatorLcdTextTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
    public:
        CInformatorLcdTextTopic()
            : CMqttTopic()
        {}
#endif

    public:
        explicit CInformatorLcdTextTopic();
        void setLcdText(const TextRC_t& lcdText);
        TextRC_t lcdText() const;

    protected:
        const QByteArray prepareData() const override;
        bool parseData(const QByteArray& data) override;

    private:
        TextRC_t m_lcdText;
};

#endif // CINFORMATORLCDTEXTTOPIC_H
