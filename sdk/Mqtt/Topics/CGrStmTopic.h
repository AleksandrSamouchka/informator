#ifndef CGRSTMTOPIC
#define CGRSTMTOPIC

#include <Mqtt/CMqttTopic.h>

class CGrStmTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrStmTopic() : CMqttTopic() {}
#endif

public:
    CGrStmTopic( const QString &topicName );
    CGrStmTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrStmTopic();
};

#endif
