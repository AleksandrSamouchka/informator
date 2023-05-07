#ifndef CSTMROBATVOLTAGETOPIC_H
#define CSTMROBATVOLTAGETOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRoBatVoltageTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
        CStmRoBatVoltageTopic();
        virtual ~CStmRoBatVoltageTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

};

#endif // CSTMROBATVOLTAGETOPIC_H
