#ifndef SDK_CLIAZFUELLEVELPRIMARYLITERSTOPIC_H
#define SDK_CLIAZFUELLEVELPRIMARYLITERSTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazFuelLevelPrimaryLitersTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazFuelLevelPrimaryLitersTopic();
    virtual ~CLiazFuelLevelPrimaryLitersTopic();
};


#endif //SDK_CLIAZFUELLEVELPRIMARYLITERSTOPIC_H
