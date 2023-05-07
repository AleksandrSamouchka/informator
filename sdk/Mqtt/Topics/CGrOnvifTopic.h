#ifndef CGRONVIFTOPIC_H
#define CGRONVIFTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrOnvifTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrOnvifTopic() : CMqttTopic() {}
#endif

public:
    CGrOnvifTopic( const QString &topicName );
    virtual ~CGrOnvifTopic();
};

#endif // CGRONVIFTOPIC_H
