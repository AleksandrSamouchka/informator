#ifndef __CSTMRWLTEPOWERTOPIC_H__
#define __CSTMRWLTEPOWERTOPIC_H__

#include <Mqtt/Topics/Stm/Rw/CStmRwLteTopic.h>

class CStmRwLtePowerTopic : public CStmRwLteTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CStmRwLtePowerTopic();
        virtual ~CStmRwLtePowerTopic();
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif
