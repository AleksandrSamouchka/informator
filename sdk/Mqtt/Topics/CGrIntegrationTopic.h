#ifndef CGRINTEGRATIONTOPIC_H
#define CGRINTEGRATIONTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrIntegrationTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrIntegrationTopic() : CMqttTopic() {}
#endif
public:
    explicit CGrIntegrationTopic( const QString &topicName );
    explicit CGrIntegrationTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrIntegrationTopic();
};

#endif // CGRINTEGRATIONTOPIC_H
