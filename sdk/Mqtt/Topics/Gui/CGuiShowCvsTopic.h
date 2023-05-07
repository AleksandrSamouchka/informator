#ifndef CGUISHOWCVSTOPIC_H
#define CGUISHOWCVSTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CGuiShowCvsTopic : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    CGuiShowCvsTopic();
};

#endif // CGUISHOWCVSTOPIC_H
