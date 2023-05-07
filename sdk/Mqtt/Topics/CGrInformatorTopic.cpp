#include <Mqtt/Topics/CGrInformatorTopic.h>

static const char * PREFIX = "INFORMATOR/";

CGrInformatorTopic::CGrInformatorTopic( const QString &topicName ) :
    CMqttTopic( PREFIX + topicName )
{
}

CGrInformatorTopic::CGrInformatorTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( PREFIX + topicName, data )
{
}

CGrInformatorTopic::~CGrInformatorTopic()
{
}

