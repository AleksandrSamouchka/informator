#include "CLiazDisabledPersonReqTopic.h"

CLiazDisabledPersonReqTopic::CLiazDisabledPersonReqTopic() :
    CSimpleTopic<int>("LIAZ/DISABLEDPERSONREQ")
{
}

CLiazDisabledPersonReqTopic::~CLiazDisabledPersonReqTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CLiazDisabledPersonReqTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
