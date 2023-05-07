#ifndef CLIAZDISABLEDPERSONREQTOPIC_H
#define CLIAZDISABLEDPERSONREQTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazDisabledPersonReqTopic : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CLiazDisabledPersonReqTopic();
        virtual ~CLiazDisabledPersonReqTopic();
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif // CLIAZDISABLEDPERSONREQTOPIC_H
