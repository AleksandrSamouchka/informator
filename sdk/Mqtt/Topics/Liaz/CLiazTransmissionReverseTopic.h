#ifndef CLIAZTRANSMISSIONREVERSETOPIC_H
#define CLIAZTRANSMISSIONREVERSETOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazTransmissionReverseTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazTransmissionReverseTopic();
    virtual ~CLiazTransmissionReverseTopic();
};

#endif // CLIAZTRANSMISSIONREVERSETOPIC_H
