#ifndef CSTMROBOARDVOLTAGETOPIC_H
#define CSTMROBOARDVOLTAGETOPIC_H


#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRoBoardVoltageTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
        CStmRoBoardVoltageTopic();
        virtual ~CStmRoBoardVoltageTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

};

#endif // CSTMROBOARDVOLTAGETOPIC_H
