#ifndef CLIAZINSIDETEMPTOPIC_H
#define CLIAZINSIDETEMPTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazInsideTempTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazInsideTempTopic();
    virtual ~CLiazInsideTempTopic();
};

#endif // CLIAZINSIDETEMPTOPIC_H
