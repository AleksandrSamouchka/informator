#ifndef CLIAZAMBIENTTEMPTOPIC_H
#define CLIAZAMBIENTTEMPTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazAmbientTempTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazAmbientTempTopic();
    virtual ~CLiazAmbientTempTopic();
};

#endif // CLIAZAMBIENTTEMPTOPIC_H
