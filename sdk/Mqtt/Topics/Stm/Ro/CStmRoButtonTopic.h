#ifndef CSTMROBUTTONTOPIC_H
#define CSTMROBUTTONTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRoButtonTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CStmRoButtonTopic();
    virtual ~CStmRoButtonTopic();
};

#endif // CSTMROBUTTONTOPIC_H
