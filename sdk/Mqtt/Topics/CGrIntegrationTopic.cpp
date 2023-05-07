#include "CGrIntegrationTopic.h"

static const char * PREFIX = "INTEGRATION/";

CGrIntegrationTopic::CGrIntegrationTopic( const QString &topicName ) :
    CMqttTopic( PREFIX + topicName )
{
}

CGrIntegrationTopic::CGrIntegrationTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( PREFIX + topicName, data )
{
}

CGrIntegrationTopic::~CGrIntegrationTopic()
{
}
