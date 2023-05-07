#include "CLiazVehStateTopic.h"

CLiazVehStateTopic::CLiazVehStateTopic() :
    CSimpleTopic< int >( "LIAZ/VEHSTATE" )
{ }

CLiazVehStateTopic::~CLiazVehStateTopic()
{ }
