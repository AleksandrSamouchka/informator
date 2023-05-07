#ifndef CGRSETTINGSTOPIC_H
#define CGRSETTINGSTOPIC_H

#include <Mqtt/CMqttTopic.h>

class CGrSettingsTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrSettingsTopic() : CMqttTopic() {}
#endif

public:
    CGrSettingsTopic( const QString &topicName );
    CGrSettingsTopic( const QString &topicName, const QByteArray &data );

    virtual ~CGrSettingsTopic();
};

#endif // CGRSETTINGSTOPIC_H
