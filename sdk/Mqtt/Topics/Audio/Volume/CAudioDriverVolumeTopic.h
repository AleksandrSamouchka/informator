#ifndef __AUDIO_DRIVER_VOLUME_TOPIC_H__
#define __AUDIO_DRIVER_VOLUME_TOPIC_H__

#include <Mqtt/Topics/Audio/CAudioVolumeTopic.h>

class CAudioDriverVolumeTopic : public CAudioVolumeTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CAudioDriverVolumeTopic();
        virtual ~CAudioDriverVolumeTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};


#endif
