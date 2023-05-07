#ifndef __CSTMHWRESETTOPIC_H__
#define __CSTMHWRESETTOPIC_H__

#include <Mqtt/Topics/CGrStmTopic.h>

class CStmHwResetTopic : public CGrStmTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CStmHwResetTopic();
        virtual ~CStmHwResetTopic();
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif
