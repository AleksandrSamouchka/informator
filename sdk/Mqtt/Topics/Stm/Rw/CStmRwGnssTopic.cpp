#include "CStmRwGnssTopic.h"

CStmRwGnssTopic::CStmRwGnssTopic(const QString &subtopicName) :
    CStmRwTopic("GNSS/" + subtopicName)
{
}

CStmRwGnssTopic::~CStmRwGnssTopic()
{
}

