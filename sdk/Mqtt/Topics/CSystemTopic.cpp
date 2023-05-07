#include "CSystemTopic.h"

static const char * PREFIX = "SYSTEM/";

CSystemTopic::CSystemTopic(const QString &subtopicName):
    CMqttTopic( PREFIX + subtopicName )
{
}

CSystemTopic::CSystemTopic(const QString & subtopicName, const QByteArray & data) :
    CMqttTopic( PREFIX + subtopicName, data )
{

}

CSystemTopic::~CSystemTopic()
{
}
