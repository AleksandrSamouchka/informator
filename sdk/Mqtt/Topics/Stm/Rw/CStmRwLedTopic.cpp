#include "CStmRwLedTopic.h"

CStmRwLedTopic::CStmRwLedTopic(int ledNum) :
    CSimpleTopic<int>( QString("STM/RW/LED%1").arg(ledNum) ),
    m_ledNum(ledNum)
{
}

CStmRwLedTopic::~CStmRwLedTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRwLedTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
