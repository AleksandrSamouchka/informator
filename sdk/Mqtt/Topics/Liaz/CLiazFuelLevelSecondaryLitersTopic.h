#ifndef SDK_CLIAZFUELLEVELSECONDARYLITERSTOPIC_H
#define SDK_CLIAZFUELLEVELSECONDARYLITERSTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazFuelLevelSecondaryLitersTopic  : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazFuelLevelSecondaryLitersTopic();
    virtual ~CLiazFuelLevelSecondaryLitersTopic();
};

#endif //SDK_CLIAZFUELLEVELSECONDARYLITERSTOPIC_H
