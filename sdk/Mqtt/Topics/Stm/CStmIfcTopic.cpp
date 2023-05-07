#include "CStmIfcTopic.h"

static const char *topic = "IFC/";

CStmIfcTopic::CStmIfcTopic(const QString &subtopicName) :
    CGrStmTopic( topic + subtopicName)
{
}

CStmIfcTopic::CStmIfcTopic(const QString &subtopicName, const QByteArray &data) :
    CGrStmTopic( topic + subtopicName, data)
{
//    m_data = data;
}


CStmIfcTopic::~CStmIfcTopic()
{
}

