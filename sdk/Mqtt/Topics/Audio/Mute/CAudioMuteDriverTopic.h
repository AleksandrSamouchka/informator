#ifndef __AUDIO_MUTE_DRIVER_TOPIC_H__
#define __AUDIO_MUTE_DRIVER_TOPIC_H__

#include <Mqtt/Topics/Audio/CAudioMuteTopic.h>

class CAudioMuteDriverTopic : public CAudioMuteTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CAudioMuteDriverTopic();
        virtual ~CAudioMuteDriverTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};


#endif
