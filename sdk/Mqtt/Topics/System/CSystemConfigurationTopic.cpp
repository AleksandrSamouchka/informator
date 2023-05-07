#include "CSystemConfigurationTopic.h"

static const char * PREFIX = "CONFIGURATION/";

#ifdef MNT_TOPICS_TEST
CSystemConfigurationTopic::CSystemConfigurationTopic() : 
    CSystemTopic( PREFIX ) 
{
}
#endif


CSystemConfigurationTopic::CSystemConfigurationTopic(const QString &subtopicName) :
    CSystemTopic( PREFIX + subtopicName )
{
}

CSystemConfigurationTopic::CSystemConfigurationTopic(const QString & subtopicName, const QByteArray & data) :
    CSystemTopic( PREFIX + subtopicName, data )
{

}

CSystemConfigurationTopic::~CSystemConfigurationTopic()
{
}

const QByteArray CSystemConfigurationTopic::prepareData() const
{
    QJsonObject jsonMainObj;

    foreach(const QString key, m_fields.keys() )
    {
        jsonMainObj.insert(key, m_fields[key].toString());
    }

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );

}
