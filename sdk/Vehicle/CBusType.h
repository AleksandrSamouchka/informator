/**
 * @author      : alexey (alexey@iskrenkov)
 * @file        : CBusType
 * @created     : Thursday Mar 26, 2020 09:15:44 +07
 */

#ifndef __CBUSTYPE_HPP__
#define __CBUSTYPE_HPP__

// Qt
#include <QString>


enum EBusType : int
{
    unknown = 0,
    nefaz5299 = 1,
    nefaz5299_intern = 2,
    nefaz6282 = 3,
    nefaz6282_intern = 4,
    nefaz5299_new = 5,
    nefaz6282_new = 6,

    liaz621365_79 = 10,
    liaz621365_80_intern = 11,
    liaz6274 = 12,
    liaz6274_intern = 13,
    liaz621365_97 = 14,
    manLionsCoachR07 = 20,
    ukvz71_628M = 30,
};

enum class EFuelType
{
    Unknown,
    Diesel,
    Electrobus
};

namespace  {
constexpr EBusType DEFAULT_BUS_TYPE{EBusType::nefaz5299};
}


namespace CBusType
{
    QString busInfoTable();
    int doorNumber(EBusType busType);

    QString toString(EBusType type);

    EBusType toType(const char *typeStr);
    QString getDescription(EBusType type);

    inline EBusType getDefault() {return DEFAULT_BUS_TYPE;}
}

#endif

