#ifndef CGRINCARNETINSPECTORTOPIC_H
#define CGRINCARNETINSPECTORTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrIncarnetInspectorTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrIncarnetInspectorTopic() : CMqttTopic() {}
#endif

public:
    CGrIncarnetInspectorTopic( const QString &topicName );
    CGrIncarnetInspectorTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrIncarnetInspectorTopic();
};

#endif // CGRINCARNETINSPECTORTOPIC_H
