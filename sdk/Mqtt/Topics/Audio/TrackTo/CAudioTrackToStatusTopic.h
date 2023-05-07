#ifndef __AUDIO_TRACK_TO_STATUS_TOPIC_H__
#define __AUDIO_TRACK_TO_STATUS_TOPIC_H__

#include <Mqtt/Topics/Audio/CAudioTrackToTopic.h>

class CAudioTrackToStatusTopic : public CAudioTrackToTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        enum class EStatuses
        {
            unknown = 0,
            queued,
            playing,
            finished,
        };


        CAudioTrackToStatusTopic();
        virtual ~CAudioTrackToStatusTopic();

        using CAudioTopic::setField;
        void setField(const char *name, EStatuses value);
        CAudioTrackToStatusTopic::EStatuses getStatus() const
        {
            return stringToStatus(getField(FIELD_STATUS).toString());
        }

    private:
        EStatuses stringToStatus(const QString &statusString) const;


    protected:
        bool parseData( const QByteArray & data ) override;

#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};


#endif
