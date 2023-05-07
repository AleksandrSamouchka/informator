#ifndef __CSTMRWOUTTOPIC_H__
#define __CSTMRWOUTTOPIC_H__

#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRwOutTopic : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CStmRwOutTopic() : CSimpleTopic<int>( QString( "STM/RW/OUT%1" ).arg( 10 )) {}
#endif
    public:
        CStmRwOutTopic(int outNum);
        virtual ~CStmRwOutTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

    private:
        int m_outNum;
};

#endif
