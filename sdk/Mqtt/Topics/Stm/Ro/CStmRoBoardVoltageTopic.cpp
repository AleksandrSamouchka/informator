#include "CStmRoBoardVoltageTopic.h"

CStmRoBoardVoltageTopic::CStmRoBoardVoltageTopic() :
    CSimpleTopic< double >( "STM/RO/BOARDVOLTAGE" )
{
}

CStmRoBoardVoltageTopic::~CStmRoBoardVoltageTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmRoBoardVoltageTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
