#include "CLiazSalonTempTopic.h"

CLiazSalonTempTopic::CLiazSalonTempTopic( int num ) :
    CSimpleTopic< double >( "LIAZ/SALOONTEMP" + QString::number( num ) ),
    m_sensorNum( num )
{ }

CLiazSalonTempTopic::~CLiazSalonTempTopic()
{ }
