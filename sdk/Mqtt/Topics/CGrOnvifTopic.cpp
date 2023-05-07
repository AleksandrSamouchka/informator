#include "CGrOnvifTopic.h"

static const char * STM_PREFIX = "ONVIF/";

CGrOnvifTopic::CGrOnvifTopic(const QString &topicName) :
    CMqttTopic( STM_PREFIX + topicName )
{ }

CGrOnvifTopic::~CGrOnvifTopic()
{ }
