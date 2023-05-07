#ifndef CLIAZMOTORTEMPTOPIC_H
#define CLIAZMOTORTEMPTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazMotorTempTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazMotorTempTopic();
    virtual ~CLiazMotorTempTopic();
};

#endif // CLIAZMOTORTEMPTOPIC_H
