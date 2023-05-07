#ifndef CLIAZTRANSMISSIONFORWARDTOPIC_H
#define CLIAZTRANSMISSIONFORWARDTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazTransmissionForwardTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazTransmissionForwardTopic();
    virtual ~CLiazTransmissionForwardTopic();
};

#endif // CLIAZTRANSMISSIONFORWARDTOPIC_H
