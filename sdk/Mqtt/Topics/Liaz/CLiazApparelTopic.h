#ifndef CLIAZAPPARELTOPIC_H
#define CLIAZAPPARELTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazApparelTopic : public CSimpleTopic<int>
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CLiazApparelTopic();
        virtual ~CLiazApparelTopic();
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif // CLIAZAPPARELTOPIC_H
