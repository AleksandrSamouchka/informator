#ifndef MQTT_STM_RW_TOPIC_H
#define MQTT_STM_RW_TOPIC_H

#include <Mqtt/Topics/CGrStmTopic.h>

class CStmRwTopic : public CGrStmTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CStmRwTopic() : CGrStmTopic( "RW/" ) {}
#endif

public:
    CStmRwTopic( const QString &topicName );
    CStmRwTopic( const QString &topicName, const QByteArray &data );

    virtual ~CStmRwTopic();
};

#endif
