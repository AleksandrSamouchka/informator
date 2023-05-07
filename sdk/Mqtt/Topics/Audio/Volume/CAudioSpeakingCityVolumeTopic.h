#ifndef __AUDIO_SPEAKING_CITY_VOLUME_TOPIC_H__
#define __AUDIO_SPEAKING_CITY_VOLUME_TOPIC_H__

#include <Mqtt/Topics/Audio/CAudioVolumeTopic.h>

class CAudioSpeakingCityVolumeTopic : public CAudioVolumeTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CAudioSpeakingCityVolumeTopic();
        virtual ~CAudioSpeakingCityVolumeTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};


#endif
