#ifndef CLIAZSOSBUTTONTOPIC_H
#define CLIAZSOSBUTTONTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazSosButtonTopic : public CSimpleTopic< int >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazSosButtonTopic();
    virtual ~CLiazSosButtonTopic();
};

#endif // CLIAZSOSBUTTONTOPIC_H
