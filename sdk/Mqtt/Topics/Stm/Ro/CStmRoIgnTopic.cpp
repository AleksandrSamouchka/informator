#include "CStmRoIgnTopic.h"

CStmRoIgnTopic::CStmRoIgnTopic() :
    CSimpleTopic< int >( "STM/RO/IGN" )
{
}

CStmRoIgnTopic::~CStmRoIgnTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRoIgnTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
