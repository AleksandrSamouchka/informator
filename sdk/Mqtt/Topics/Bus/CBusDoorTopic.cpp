#include "CBusDoorTopic.h"

static const char *doorTopicName = "LIAZ/DOOR";

CBusDoorTopic::CBusDoorTopic(int num) :
    CSimpleTopic<int>(QString("%1%2").arg(doorTopicName).arg(num)),
    m_doorNum(num)
{
}

CBusDoorTopic::~CBusDoorTopic()
{
}
