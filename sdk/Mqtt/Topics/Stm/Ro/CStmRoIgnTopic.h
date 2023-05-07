#ifndef CSTMROIGNTOPIC_H
#define CSTMROIGNTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CStmRoIgnTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CStmRoIgnTopic();
    virtual ~CStmRoIgnTopic();
#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif // CSTMROIGNTOPIC_H
