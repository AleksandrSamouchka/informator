#include "CLiazApparelTopic.h"

CLiazApparelTopic::CLiazApparelTopic() :
    CSimpleTopic<int>("LIAZ/APPAREL")
{
}

CLiazApparelTopic::~CLiazApparelTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CLiazApparelTopic::getEtalonJson()
{
    return QByteArray("RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW");
}
#endif
