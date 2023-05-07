#include "CLiazMotorTempTopic.h"

CLiazMotorTempTopic::CLiazMotorTempTopic() :
    CSimpleTopic< double >( "LIAZ/MOTORTEMP" )
{ }

CLiazMotorTempTopic::~CLiazMotorTempTopic()
{ }
