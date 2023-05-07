#include "CGrValidatorsTopic.h"

static const char * PREFIX = "VALIDATORS/";

CGrValidatorsTopic::CGrValidatorsTopic(const QString &topicName) :
    CMqttTopic( PREFIX + topicName )
{
}

CGrValidatorsTopic::CGrValidatorsTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( PREFIX + topicName, data )
{
}

CGrValidatorsTopic::~CGrValidatorsTopic()
{
}
