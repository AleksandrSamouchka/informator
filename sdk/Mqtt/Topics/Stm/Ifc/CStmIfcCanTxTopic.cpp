#include "CStmIfcCanTxTopic.h"

static const char *fmt = "CAN%1TX";

CStmIfcCanTxTopic::CStmIfcCanTxTopic(int canId) :
    CStmIfcTopic(QString(fmt).arg(canId)),
    m_canId(canId)
{
}

CStmIfcCanTxTopic::CStmIfcCanTxTopic(int canId, const QByteArray &data) :
    CStmIfcTopic(QString(fmt).arg(canId), data),
    m_canId(canId)
{
}


CStmIfcCanTxTopic::~CStmIfcCanTxTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmIfcCanTxTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
