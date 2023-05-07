#include "CLiazBoardVoltageTopic.h"

CLiazBoardVoltageTopic::CLiazBoardVoltageTopic() :
    CSimpleTopic< double >( "LIAZ/VOLTAGE" )
{ }

CLiazBoardVoltageTopic::~CLiazBoardVoltageTopic()
{ }
