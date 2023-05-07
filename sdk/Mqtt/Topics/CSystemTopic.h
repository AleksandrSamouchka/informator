#ifndef SYSTEM_TOPIC_H
#define SYSTEM_TOPIC_H

#include <Mqtt/CMqttTopic.h>

class CSystemTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CSystemTopic() : CMqttTopic() {}
#endif
    public:
        explicit CSystemTopic(const QString &subtopicName);
        explicit CSystemTopic( const QString &subtopicName, const QByteArray &data );

        virtual ~CSystemTopic();
};


#endif

