#ifndef CLIAZFUELLEVELSECONDARYTOPIC_H
#define CLIAZFUELLEVELSECONDARYTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazFuelLevelSecondaryTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazFuelLevelSecondaryTopic();
    virtual ~CLiazFuelLevelSecondaryTopic();
};

#endif // CLIAZFUELLEVELSECONDARYTOPIC_H
