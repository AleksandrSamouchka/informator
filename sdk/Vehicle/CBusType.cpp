/**
 * @author      : alexey (alexey@iskrenkov)
 * @file        : CBusType
 * @created     : Thursday Mar 26, 2020 09:15:35 +07
 */

// Local
#include "CBusType.h"

// Qt
#include <QMap>


namespace  {

struct BusInfo {
    QString     name{"<НЕИЗВЕСТНО>"};
    EFuelType   fuel{EFuelType::Unknown};
    int         doorCount{0};
    const char* typeStr{"<unknown>"};
};

const QMap<EBusType, BusInfo> BUS_INFO_TABLE{
    {EBusType::nefaz5299,            {QStringLiteral("НЕФАЗ 5299"),              EFuelType::Diesel,     3, "nefaz5299"}},
    {EBusType::nefaz5299_intern,     {QStringLiteral("НЕФАЗ 5299 (Стажер)"),     EFuelType::Diesel,     3, "nefaz5299_intern"}},
    {EBusType::nefaz6282,            {QStringLiteral("НЕФАЗ 6282"),              EFuelType::Electrobus, 3, "nefaz6282"}},
    {EBusType::nefaz6282_intern,     {QStringLiteral("НЕФАЗ 6282 (Стажер)"),     EFuelType::Electrobus, 3, "nefaz6282_intern"}},
    {EBusType::nefaz5299_new,        {QStringLiteral("НЕФАЗ 5299 (Новый)"),      EFuelType::Diesel,     3, "nefaz5299_new"}},
    {EBusType::nefaz6282_new,        {QStringLiteral("НЕФАЗ 6282 (Новый)"),      EFuelType::Electrobus, 3, "nefaz6282_new"}},
    {EBusType::liaz621365_79,        {QStringLiteral("ЛИАЗ 621365-79"),          EFuelType::Diesel,     4, "liaz621365_79"}},
    {EBusType::liaz621365_80_intern, {QStringLiteral("ЛИАЗ 621365-80 (Стажер)"), EFuelType::Diesel,     4, "liaz621365_80_intern"}},
    {EBusType::liaz6274,             {QStringLiteral("ЛИАЗ 6274"),               EFuelType::Electrobus, 3, "liaz6274"}},
    {EBusType::liaz6274_intern,      {QStringLiteral("ЛИАЗ 6274 (Стажер)"),      EFuelType::Electrobus, 3, "liaz6274_intern"}},
    {EBusType::liaz621365_97,        {QStringLiteral("ЛИАЗ 621365-97"),          EFuelType::Diesel,     4, "liaz621365-97"}},
    {EBusType::manLionsCoachR07,     {QStringLiteral("МАN LION'S COACH R07"),    EFuelType::Diesel,     2, "manLionsCoachR07"}},
    {EBusType::ukvz71_628M,          {QStringLiteral("УКВЗ Трамвай 71-628М"),    EFuelType::Unknown,    4, "ukvz71-628М"}},

};

QString generateBusTable(const QMap<EBusType, BusInfo>& table)
{
    QString res;
    const auto maxName =
            std::max_element(table.begin(),
                             table.end(),
                             [](const auto& a, const auto& b) {return a.name.size() < b.name.size();})->name.size();
    for(auto it{table.begin()};; res.append(QLatin1String("\n")))
    {
        res.append(QStringLiteral("\t%1 =%2 | %3 %4 двери |")
                   .arg(it->name, -(1 + maxName))
                   .arg(static_cast<int>(it.key()), 2)
                   .arg(it->fuel == EFuelType::Diesel ? QStringLiteral("дизель,") : QStringLiteral("электробус,"), -10)
                   .arg(it->doorCount)
                   );
        if(++it == table.end())
            return res;
    }
};
}

QString CBusType::busInfoTable()
{
    static const QString BUS_TABLE{generateBusTable(BUS_INFO_TABLE)};
    return BUS_TABLE;
}

int CBusType::doorNumber(EBusType busType)
{
    return BUS_INFO_TABLE.value(busType).doorCount;
}

QString CBusType::toString(EBusType type)
{
    return BUS_INFO_TABLE.value(type).typeStr;
}

EBusType CBusType::toType(const char *typeStr)
{
    auto info{std::find_if(BUS_INFO_TABLE.begin(), BUS_INFO_TABLE.end(),
                 [typeStr](const auto& a){return a.typeStr == typeStr;})};

    return (info == BUS_INFO_TABLE.end()) ? EBusType::unknown : info.key();
}

QString CBusType::getDescription(EBusType type)
{
    return BUS_INFO_TABLE.value(type).name;
}


