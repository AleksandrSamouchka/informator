#ifndef CSTMRWLEDTOPIC_H
#define CSTMRWLEDTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRwLedTopic : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CStmRwLedTopic() : CSimpleTopic<int>(QString("STM/RW/LED%1").arg( 10 )) {}
#endif
    public:
        CStmRwLedTopic(int ledNum);
        virtual ~CStmRwLedTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

    private:
        int m_ledNum;
};

#endif
