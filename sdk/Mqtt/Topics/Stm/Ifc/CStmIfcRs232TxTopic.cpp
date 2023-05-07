#include "CStmIfcRs232TxTopic.h"

static const char *fmt = "RS232%1TX";

CStmIfcRs232TxTopic::CStmIfcRs232TxTopic(int rsId) :
    CStmIfcTopic(QString(fmt).arg(rsId)),
    m_rsId(rsId)
{
}

CStmIfcRs232TxTopic::CStmIfcRs232TxTopic(int rsId, const QByteArray &data) :
    CStmIfcTopic(QString(fmt).arg(rsId), data),
    m_rsId(rsId)
{
}


CStmIfcRs232TxTopic::~CStmIfcRs232TxTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmIfcRs232TxTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
