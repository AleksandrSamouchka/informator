#include "CLiazDoorOpeningTopic.h"

CLiazDoorOpeningTopic::CLiazDoorOpeningTopic(int num) :
    CSimpleTopic<int>( QString("LIAZ/DOOR%1OPENING").arg( num ) ),
    m_doorNum( num )
{
}

CLiazDoorOpeningTopic::~CLiazDoorOpeningTopic()
{
}
