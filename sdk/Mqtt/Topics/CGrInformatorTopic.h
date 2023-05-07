#ifndef MQTT_INFORMATORGR_TOPIC_H
#define MQTT_INFORMATORGR_TOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrInformatorTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrInformatorTopic() : CMqttTopic() {}
#endif
public:
    explicit CGrInformatorTopic( const QString &topicName );
    explicit CGrInformatorTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrInformatorTopic();
};

#endif
