#ifndef CLIAZBOARDVOLTAGETOPIC_H
#define CLIAZBOARDVOLTAGETOPIC_H

#include <Mqtt/Topics/CSimpleTopic.h>

class CLiazBoardVoltageTopic : public CSimpleTopic< double >
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    CLiazBoardVoltageTopic();
    virtual ~CLiazBoardVoltageTopic();
};

#endif // CLIAZBOARDVOLTAGETOPIC_H
