#ifndef CGRCONNECTIONTOPIC_H
#define CGRCONNECTIONTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrConnectionTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrConnectionTopic() : CMqttTopic() {}
#endif
public:
    CGrConnectionTopic( const QString &topicName );
    CGrConnectionTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrConnectionTopic();
};

#endif // CGRCONNECTIONTOPIC_H
