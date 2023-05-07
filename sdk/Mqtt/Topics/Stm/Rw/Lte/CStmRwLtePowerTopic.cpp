#include "CStmRwLtePowerTopic.h"

CStmRwLtePowerTopic::CStmRwLtePowerTopic() :
    CStmRwLteTopic("POWER")
{
}

CStmRwLtePowerTopic::~CStmRwLtePowerTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRwLtePowerTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
