#ifndef CTOPICTEXTFORLED_H
#define CTOPICTEXTFORLED_H

// Qt
#include <QString>

// SDK
#include <Mqtt/CMqttTopic.h>
#include "Mqtt/Topics/Informator/RouteNetworkStructures.h"

class CTopicTextForLed : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
    public:
        CTopicTextForLed()
            : CMqttTopic()
        {}
#endif

    public:

        enum TextForLedMsgType
        {
           EmergencyMsg,
           TextMsg
        };

        explicit CTopicTextForLed();
        TextRC_t ledNum() const;
        TextRC_t ledSide() const;
        TextRC_t ledFront() const;
        TextRC_t ledInternal() const;
        TextRC_t lcdText() const;
        TextForLedMsgType msgType() const;

        void setLedNum(const TextRC_t& ledNum);
        void setLedSide(const TextRC_t& ledSide);
        void setLedFront(const TextRC_t& ledFront);
        void setLedInternal(const TextRC_t& ledInternal);
        void setLcdText(const TextRC_t& lcdText);
        void setMsgType(TextForLedMsgType msgType);

    protected:
        const QByteArray prepareData() const override;
        bool parseData(const QByteArray& data) override;

    private:
        QString TextForLedMsgTypeToStr(TextForLedMsgType msgType) const;
        TextForLedMsgType TextForLedMsgTypeFromStr(const QString& str);

    private:
        TextRC_t m_ledNum;
        TextRC_t m_ledSide;
        TextRC_t m_ledFront;
        TextRC_t m_ledInternal;
        TextRC_t m_lcdText;
        TextForLedMsgType m_msgType;
};

#endif // CTOPICTEXTFORLED_H
