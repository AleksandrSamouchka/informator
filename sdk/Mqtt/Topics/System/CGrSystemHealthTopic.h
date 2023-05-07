#ifndef CGRSYSTEMHEALTHTOPIC_H
#define CGRSYSTEMHEALTHTOPIC_H

#include <Mqtt/Topics/CSystemTopic.h>

class CGrSystemHealthTopic : public CSystemTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrSystemHealthTopic() : CMqttTopic() {}
#endif
public:
    explicit CGrSystemHealthTopic(const QString &subtopicName);

    virtual ~CGrSystemHealthTopic();
};

#endif // CGRSYSTEMHEALTHTOPIC_H
