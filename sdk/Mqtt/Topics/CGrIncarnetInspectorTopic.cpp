#include "CGrIncarnetInspectorTopic.h"

static const char * PREFIX = "INCARNET_INSPECTOR/";

CGrIncarnetInspectorTopic::CGrIncarnetInspectorTopic(const QString &topicName) :
    CMqttTopic( PREFIX + topicName )
{
}

CGrIncarnetInspectorTopic::CGrIncarnetInspectorTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( PREFIX + topicName, data )
{
}

CGrIncarnetInspectorTopic::~CGrIncarnetInspectorTopic()
{
}
