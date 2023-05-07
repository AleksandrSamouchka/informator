#include "CStmRwGnssBackupTopic.h"

CStmRwGnssBackupTopic::CStmRwGnssBackupTopic() :
    CStmRwGnssTopic("BACKUP")
{
}

CStmRwGnssBackupTopic::~CStmRwGnssBackupTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRwGnssBackupTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
