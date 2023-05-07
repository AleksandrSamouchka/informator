#include "CStmHwResetTopic.h"

CStmHwResetTopic::CStmHwResetTopic() :
    CGrStmTopic("HW_RESET")
{
}

CStmHwResetTopic::~CStmHwResetTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmHwResetTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
