#ifndef CGUISOSTOPIC_H
#define CGUISOSTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CGuiSosTopic : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    CGuiSosTopic();
};

#endif // CGUISOSTOPIC_H
