#include "CStmRwGnssPowerTopic.h"

CStmRwGnssPowerTopic::CStmRwGnssPowerTopic() :
    CStmRwGnssTopic("POWER")
{
}

CStmRwGnssPowerTopic::~CStmRwGnssPowerTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRwGnssPowerTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
