#include "CStmUpdateFwTopic.h"

CStmUpdateFwTopic::CStmUpdateFwTopic() :
    CGrStmTopic("UPDATE_FIRMWARE")
{
}

CStmUpdateFwTopic::~CStmUpdateFwTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmUpdateFwTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
