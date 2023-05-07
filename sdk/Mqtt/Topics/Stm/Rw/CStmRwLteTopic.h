#ifndef __CSTMRWLTETOPIC_H__
#define __CSTMRWLTETOPIC_H__

#include <Mqtt/Topics/Stm/CStmRwTopic.h>

class CStmRwLteTopic : public CStmRwTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CStmRwLteTopic() : CStmRwTopic("LTE/") {}
#endif
    public:
        CStmRwLteTopic(const QString &subtopicName);
        virtual ~CStmRwLteTopic();
};

#endif
