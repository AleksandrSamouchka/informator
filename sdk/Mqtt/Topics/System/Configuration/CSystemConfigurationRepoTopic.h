#ifndef __CSYSTEMCONFIGURATIONREPOTOPIC_H__
#define __CSYSTEMCONFIGURATIONREPOTOPIC_H__

#include <Mqtt/Topics/System/CSystemConfigurationTopic.h>

class CSystemConfigurationRepoTopic : public CSystemConfigurationTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
    public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
        CSystemConfigurationRepoTopic();
        virtual ~CSystemConfigurationRepoTopic();
};

#endif
