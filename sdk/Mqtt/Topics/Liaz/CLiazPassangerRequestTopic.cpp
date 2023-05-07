#include "CLiazPassangerRequestTopic.h"

CLiazPassangerRequestTopic::CLiazPassangerRequestTopic(int num) :
    CSimpleTopic<int>( "LIAZ/PASSENGERREQ" + QString::number( num ) ),
    m_doorNum( num )
{
}

CLiazPassangerRequestTopic::~CLiazPassangerRequestTopic()
{
}
