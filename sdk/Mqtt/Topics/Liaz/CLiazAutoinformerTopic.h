#ifndef CLIAZAUTOINFORMERTOPIC_H
#define CLIAZAUTOINFORMERTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazAutoinformerTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazAutoinformerTopic();
    virtual ~CLiazAutoinformerTopic();
};

#endif // CLIAZAUTOINFORMERTOPIC_H
