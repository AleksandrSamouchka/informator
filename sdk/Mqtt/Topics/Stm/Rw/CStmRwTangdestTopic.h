#ifndef CSTMRWTANGDESTTOPIC_H
#define CSTMRWTANGDESTTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRwTangdestTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CStmRwTangdestTopic();
    virtual ~CStmRwTangdestTopic();
};

#endif
