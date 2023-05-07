#ifndef __AUDIO_VOLUME_TOPIC_H__
#define __AUDIO_VOLUME_TOPIC_H__

#include <Mqtt/Topics/CAudioTopic.h>

class CAudioVolumeTopic : public CAudioTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CAudioVolumeTopic() : CAudioTopic() {}
#endif

    public:
        CAudioVolumeTopic(const char *subTopicName);
        virtual ~CAudioVolumeTopic();
};


#endif
