#ifndef CSTMROTAMPERTOPIC_H
#define CSTMROTAMPERTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRoTamperTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CStmRoTamperTopic();
    virtual ~CStmRoTamperTopic();
};

#endif // CSTMROTAMPERTOPIC_H
