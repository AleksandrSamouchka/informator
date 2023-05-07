#include "CGrRecorderTopic.h"

static const char * PREFIX = "RECORDER/";

CGrRecorderTopic::CGrRecorderTopic( const QString & topicName ) :
    CMqttTopic( PREFIX + topicName )
{ }

CGrRecorderTopic::~CGrRecorderTopic()
{ }
