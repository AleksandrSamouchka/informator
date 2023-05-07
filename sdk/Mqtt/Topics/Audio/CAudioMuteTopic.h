#ifndef __AUDIO_MUTE_TOPIC_H__
#define __AUDIO_MUTE_TOPIC_H__

#include <Mqtt/Topics/CAudioTopic.h>

class CAudioMuteTopic : public CAudioTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CAudioMuteTopic() : CAudioTopic() {}
#endif

    public:
        CAudioMuteTopic(const char *subTopicName);
        virtual ~CAudioMuteTopic();
};
/*
class CAudioMuteDriverTopic : public CAudioMuteTopic
{
    Q_OBJECT

    public:
        CAudioMuteDriverTopic();
        virtual ~CAudioMuteDriverTopic();
};

class CAudioMuteSalonTopic : public CAudioMuteTopic
{
    Q_OBJECT

    public:
        CAudioMuteSalonTopic();
        virtual ~CAudioMuteSalonTopic();
};

class CAudioMuteSpeakingCityTopic : public CAudioMuteTopic
{
    Q_OBJECT

    public:
        CAudioMuteSpeakingCityTopic();
        virtual ~CAudioMuteSpeakingCityTopic();
};
*/

#endif
