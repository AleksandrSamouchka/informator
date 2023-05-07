#ifndef CEMERGENCYALERTTOPIC_H
#define CEMERGENCYALERTTOPIC_H

// Qt
#include <QStringList>
#include <QVector>

// SDK
#include <Mqtt/CMqttTopic.h>
#include "Mqtt/Topics/Informator/RouteNetworkStructures.h"


class CEmergencyAlertTopic  : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
    public:
        CEmergencyAlertTopic()
            : CMqttTopic()
        {}
#endif

    public:

         enum MsgType
         {
            PlayMsg,
            StopMsg
         };

        explicit CEmergencyAlertTopic(const QString& depot_id);
        int period() const;
        int& period();
        bool isPeriodic() const;
        unsigned int id() const;
        MsgType msgType() const;
        QVector<Audio_t> audioFiles() const;

        TextRC_t ledNum() const;
        TextRC_t ledSide() const;
        TextRC_t ledFront() const;
        TextRC_t ledInternal() const;
        TextRC_t lcdText() const;

    protected:
        const QByteArray prepareData() const override;
        bool parseData(const QByteArray& data) override;

    private:
        //! Периодичность оповещения: TRUE – оповещение воспроизводится периодически, с количеством period раз
        bool m_isPeriodic;
        //! Количество раз воспроизведения
        int m_period;
        //! Идентификатор аварийного сообщения
        unsigned int m_id;
        QByteArray m_rawData;
        QVector<Audio_t> m_audioFiles;
        MsgType m_msgType;
        TextRC_t m_ledNum;
        TextRC_t m_ledSide;
        TextRC_t m_ledFront;
        TextRC_t m_ledInternal;
        TextRC_t m_lcdText;
};

#endif // CEMERGENCYALERTTOPIC_H
