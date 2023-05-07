#ifndef __CSTMUPDATEFWTOPIC_H__
#define __CSTMUPDATEFWTOPIC_H__

#include <Mqtt/Topics/CGrStmTopic.h>

class CStmUpdateFwTopic : public CGrStmTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CStmUpdateFwTopic();
        virtual ~CStmUpdateFwTopic();
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
};

#endif
