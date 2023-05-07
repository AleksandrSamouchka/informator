#include "CStmIfcRs232RxTopic.h"

CStmIfcRs232RxTopic::CStmIfcRs232RxTopic(int rsId) :
    CStmIfcTopic(QString("RS232%1RX").arg(rsId)),
    m_rsId(rsId)
{
}

CStmIfcRs232RxTopic::~CStmIfcRs232RxTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmIfcRs232RxTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
