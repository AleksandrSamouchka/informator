#ifndef CLIAZOILTEMPTOPIC_H
#define CLIAZOILTEMPTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazOilTempTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazOilTempTopic();
    virtual ~CLiazOilTempTopic();
};

#endif // CLIAZOILTEMPTOPIC_H
