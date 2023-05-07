#include "CStmIfcRs485RxTopic.h"

CStmIfcRs485RxTopic::CStmIfcRs485RxTopic(int rsId) :
    CStmIfcTopic(QString("RS485%1RX").arg(rsId)),
    m_rsId(rsId)
{
}

CStmIfcRs485RxTopic::~CStmIfcRs485RxTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmIfcRs485RxTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
