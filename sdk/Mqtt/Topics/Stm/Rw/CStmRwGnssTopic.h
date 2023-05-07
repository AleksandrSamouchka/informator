#ifndef __CSTMRWGNSSTOPIC_H__
#define __CSTMRWGNSSTOPIC_H__

#include <Mqtt/Topics/Stm/CStmRwTopic.h>

class CStmRwGnssTopic : public CStmRwTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CStmRwGnssTopic() : CStmRwTopic("GNSS/") {}
#endif
    public:
        CStmRwGnssTopic(const QString &subtopicName);
        virtual ~CStmRwGnssTopic();
};

#endif
