#include "CStmRwOutTopic.h"

CStmRwOutTopic::CStmRwOutTopic(int outNum) :
    CSimpleTopic<int>(QString("STM/RW/OUT%1").arg(outNum)),
    m_outNum(outNum)
{
}

CStmRwOutTopic::~CStmRwOutTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRwOutTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
