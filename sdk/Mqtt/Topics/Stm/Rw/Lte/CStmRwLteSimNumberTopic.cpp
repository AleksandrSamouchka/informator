#include "CStmRwLteSimNumberTopic.h"

CStmRwLteSimNumberTopic::CStmRwLteSimNumberTopic() :
    CSimpleTopic<int>("STM/RW/LTE/SIMNUMBER")
{
}

CStmRwLteSimNumberTopic::~CStmRwLteSimNumberTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRwLteSimNumberTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
