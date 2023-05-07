#include "informator_defines.h"

bool operator==(DetectedStation_t one, DetectedStation_t two)
{
    return one.stationIndex == two.stationIndex &&
            one.subrouteIndex == two.subrouteIndex;
}
