#ifndef CLIAZCOOLANTTEMPTOPIC_H
#define CLIAZCOOLANTTEMPTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazCoolantTempTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazCoolantTempTopic();
    virtual ~CLiazCoolantTempTopic();
};

#endif // CLIAZCOOLANTTEMPTOPIC_H
