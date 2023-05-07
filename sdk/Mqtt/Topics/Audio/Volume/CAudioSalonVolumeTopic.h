#ifndef __AUDIO_SALON_VOLUME_TOPIC_H__
#define __AUDIO_SALON_VOLUME_TOPIC_H__

#include <Mqtt/Topics/Audio/CAudioVolumeTopic.h>

class CAudioSalonVolumeTopic : public CAudioVolumeTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CAudioSalonVolumeTopic();
        virtual ~CAudioSalonVolumeTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif
