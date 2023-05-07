#ifndef CGRVALIDATORSTOPIC_H
#define CGRVALIDATORSTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrValidatorsTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrValidatorsTopic() : CMqttTopic() {}
#endif

public:
    CGrValidatorsTopic( const QString &topicName );
    CGrValidatorsTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrValidatorsTopic();
};

#endif // CGRVALIDATORSTOPIC_H
