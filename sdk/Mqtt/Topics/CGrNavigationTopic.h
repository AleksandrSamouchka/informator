#ifndef CGRNAVIGATIONTOPIC_H
#define CGRNAVIGATIONTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrNavigationTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrNavigationTopic() : CMqttTopic() {}
#endif
public:
    explicit CGrNavigationTopic( const QString &topicName );
    explicit CGrNavigationTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrNavigationTopic();
};

#endif // CGRNAVIGATIONTOPIC_H
