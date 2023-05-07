#include "CStmIfcRs485TxTopic.h"

static const char *fmt = "RS485%1TX";

CStmIfcRs485TxTopic::CStmIfcRs485TxTopic(int rsId) :
    CStmIfcTopic(QString(fmt).arg(rsId)),
    m_rsId(rsId)
{
}

CStmIfcRs485TxTopic::CStmIfcRs485TxTopic(int rsId, const QByteArray &data) :
    CStmIfcTopic(QString(fmt).arg(rsId), data),
    m_rsId(rsId)
{
}


CStmIfcRs485TxTopic::~CStmIfcRs485TxTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmIfcRs485TxTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
