#ifndef CLIAZTAHOSPEEDTOPIC_H
#define CLIAZTAHOSPEEDTOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazTahospeedTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazTahospeedTopic();
    virtual ~CLiazTahospeedTopic();
};

#endif // CLIAZTAHOSPEEDTOPIC_H
