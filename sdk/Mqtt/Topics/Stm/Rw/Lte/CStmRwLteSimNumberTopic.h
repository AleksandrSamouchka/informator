#ifndef CSTMRWLTESIMNUMBERTOPIC_H
#define CSTMRWLTESIMNUMBERTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRwLteSimNumberTopic : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CStmRwLteSimNumberTopic();
        virtual ~CStmRwLteSimNumberTopic();
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif
