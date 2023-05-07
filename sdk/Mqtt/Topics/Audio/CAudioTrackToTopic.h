#ifndef __AUDIO_TRACK_TO_TOPIC_H__
#define __AUDIO_TRACK_TO_TOPIC_H__

#include <Mqtt/Topics/CAudioTopic.h>



class CAudioTrackToTopic : public CAudioTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
#endif

    public:

        enum class EAction
        {
            unknown = 0,
            start,
            stop,
        };

        enum class EPriority
        {
            low = 0,
            hight
        };

        CAudioTrackToTopic();
        CAudioTrackToTopic(const QString &subTopicName);
        virtual ~CAudioTrackToTopic() override;

        using CAudioTopic::setField; 
        void setField(const char *name, EAction value);
        CAudioTrackToTopic::EAction getAction() const
        {
            return stringToAction(getField(FIELD_ACTION).toString());
        }

    private:
        EAction stringToAction(const QString &actionString) const;

    protected:
        virtual bool parseData( const QByteArray & data );
        const QByteArray prepareData() const override;
#ifdef MNT_TOPICS_TEST
public:
      QByteArray getEtalonJson( const QByteArray & data );
#endif
};


#endif
