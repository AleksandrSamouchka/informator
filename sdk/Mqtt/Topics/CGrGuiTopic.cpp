#include "CGrGuiTopic.h"

static const char * PREFIX = "GUI/";

CGrGuiTopic::CGrGuiTopic( const QString &topicName ) :
    CMqttTopic( PREFIX + topicName )
{
}

CGrGuiTopic::CGrGuiTopic( const QString &topicName, const QByteArray & data) :
    CMqttTopic( PREFIX + topicName, data )
{
}

CGrGuiTopic::~CGrGuiTopic()
{
}
