#ifndef __AUDIO_TANGDEST_TOPIC_H__
#define __AUDIO_TANGDEST_TOPIC_H__

#include <Mqtt/Topics/CAudioTopic.h>

class CAudioTangDestTopic : public CAudioTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
#endif

    public:
        CAudioTangDestTopic();
//        CAudioTangDestTopic(const char *subTopicName);
        virtual ~CAudioTangDestTopic();

    protected:
        virtual bool parseData( const QByteArray & data );
        const QByteArray prepareData() const override;

};


#endif
