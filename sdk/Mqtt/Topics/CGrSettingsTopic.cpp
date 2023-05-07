#include "CGrSettingsTopic.h"

static const char * PREFIX = "SETTINGS/";

CGrSettingsTopic::CGrSettingsTopic(const QString &topicName) :
    CMqttTopic( PREFIX + topicName )
{
}

CGrSettingsTopic::CGrSettingsTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( PREFIX + topicName, data )
{
}

CGrSettingsTopic::~CGrSettingsTopic()
{
}
