#ifndef __CSTMRWGNSSPOWERTOPIC_H__
#define __CSTMRWGNSSPOWERTOPIC_H__

#include <Mqtt/Topics/Stm/Rw/CStmRwGnssTopic.h>

class CStmRwGnssPowerTopic : public CStmRwGnssTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CStmRwGnssPowerTopic();
        virtual ~CStmRwGnssPowerTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif
