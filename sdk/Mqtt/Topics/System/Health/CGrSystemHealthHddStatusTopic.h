#ifndef CGRSYSTEMHEALTHHDDSTATUSTOPIC_H
#define CGRSYSTEMHEALTHHDDSTATUSTOPIC_H

#include <Mqtt/Topics/System/CGrSystemHealthTopic.h>

class CGrSystemHealthHddStatusTopic : public CGrSystemHealthTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrSystemHealthHddStatusTopic() : CMqttTopic() {}
#endif
public:
    explicit CGrSystemHealthHddStatusTopic( const QString & subtopicName );

    virtual ~CGrSystemHealthHddStatusTopic();
};

#endif // CGRSYSTEMHEALTHHDDSTATUSTOPIC_H
