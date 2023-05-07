#include "CGuiSosTopic.h"

static const char *TOPIC = "GUI/SOS";

CGuiSosTopic::CGuiSosTopic() :
    CSimpleTopic<int>( TOPIC )
{ }
