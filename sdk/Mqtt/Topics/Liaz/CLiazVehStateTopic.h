#ifndef CLIAZVEHSTATETOPIC_H
#define CLIAZVEHSTATETOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazVehStateTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazVehStateTopic();
    virtual ~CLiazVehStateTopic();
};

#endif // CLIAZVEHSTATETOPIC_H
