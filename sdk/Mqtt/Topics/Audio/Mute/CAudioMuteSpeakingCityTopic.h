#ifndef __AUDIO_MUTE_SPEAKING_CITY_TOPIC_H__
#define __AUDIO_MUTE_SPEAKING_CITY_TOPIC_H__

#include <Mqtt/Topics/Audio/CAudioMuteTopic.h>

class CAudioMuteSpeakingCityTopic : public CAudioMuteTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CAudioMuteSpeakingCityTopic();
        virtual ~CAudioMuteSpeakingCityTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE virtual QByteArray getEtalonJson() override;
#endif
};


#endif
