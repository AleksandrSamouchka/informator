#include "CStmRoBatVoltageTopic.h"

CStmRoBatVoltageTopic::CStmRoBatVoltageTopic() :
    CSimpleTopic< double >( "STM/RO/BATVOLTAGE" )
{
}

CStmRoBatVoltageTopic::~CStmRoBatVoltageTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRoBatVoltageTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
