#include "CLiazDoorClosingTopic.h"

CLiazDoorClosingTopic::CLiazDoorClosingTopic(int num) :
    CSimpleTopic<int>( QString("LIAZ/DOOR%1CLOSING").arg( num ) ),
    m_doorNum( num )
{ }

CLiazDoorClosingTopic::~CLiazDoorClosingTopic()
{ }
