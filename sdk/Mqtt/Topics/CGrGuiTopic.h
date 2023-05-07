#ifndef CGRGUITOPIC_H
#define CGRGUITOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrGuiTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrGuiTopic() : CMqttTopic() {}
#endif
public:
    explicit CGrGuiTopic( const QString &topicName );
    explicit CGrGuiTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrGuiTopic();
};

#endif // CGRGUITOPIC_H
