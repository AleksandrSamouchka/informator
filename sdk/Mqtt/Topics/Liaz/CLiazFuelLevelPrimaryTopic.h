#ifndef CLIAZFUELLEVELPRIMARYTOPIC_H
#define CLIAZFUELLEVELPRIMARYTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazFuelLevelPrimaryTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazFuelLevelPrimaryTopic();
    virtual ~CLiazFuelLevelPrimaryTopic();
};

#endif // CLIAZFUELLEVELPRIMARYTOPIC_H
