#ifndef CGRLIAZTOPIC_H
#define CGRLIAZTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrLiazTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrLiazTopic() : CMqttTopic() {}
#endif

public:
    CGrLiazTopic( const QString &topicName );
    CGrLiazTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrLiazTopic();
};

#endif // CGRLIAZTOPIC_H
