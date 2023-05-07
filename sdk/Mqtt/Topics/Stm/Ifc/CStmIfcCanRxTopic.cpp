#include "CStmIfcCanRxTopic.h"

CStmIfcCanRxTopic::CStmIfcCanRxTopic(int canId) :
    CStmIfcTopic(QString("CAN%1RX").arg(canId)),
    m_canId(canId)
{
}

CStmIfcCanRxTopic::~CStmIfcCanRxTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CStmIfcCanRxTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
