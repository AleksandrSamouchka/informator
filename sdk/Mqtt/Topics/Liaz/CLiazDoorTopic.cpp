#include "CLiazDoorTopic.h"

static const QString doorTopicName = "LIAZ/DOOR";

CLiazDoorTopic::CLiazDoorTopic(int num) :
    CSimpleTopic<int>( doorTopicName + QString::number( num ) ),
    m_doorNum( num )
{
}

CLiazDoorTopic::~CLiazDoorTopic()
{
}
