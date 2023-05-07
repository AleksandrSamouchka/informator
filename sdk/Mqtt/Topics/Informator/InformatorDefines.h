#pragma once

// Qt
#include <QString>
#include <QVector>
#include <QDateTime>

// SDK
#include <Mqtt/Topics/Informator/RouteNetworkStructures.h>

//! Ошибки информатора
enum class InformatorError_t: uint8_t
{
    noError,
    //! Указан неверный индекс маршрута
    wrongRouteIndexError,
    //! Указан неверный индекс подмаршрута
    wrongSubrouteIndexError
};

//! Режим работы информатора
enum class InformatorWorkMode_t : uint8_t
{
    Auto,
    Unknown
};

//! Группы динамиков звукового оповещения
enum class SoundInfoType_t : uint8_t
{
    INSIDE_INFORMER_GROUP,
    OUTSIDE_INFORMER_GROUP,
    unknownValue
};

//! Режим работы информатора
enum class InformatorRouteType_t : uint8_t
{
    bus,				// автобус
    trolleybus,			// тролейбус
    train,              // Поезд
    fixedRouteTaxi,		// Маршрутное такси
    tram, 				// трамвай

    // Затем добавленные виды транспорта (разделены, чтобы сохранить соовместимость
    // видов транспорта по значениям)
    aeroexpress,        // аэроекспресс
    waterTransport,     // водный транспорт
    cableCar,           // Канатная дорога
    lightMetro,         // Легкое метро
    metro,              // метро
    metrotram,          // Метротрам
    monoreil,           // Монорельс
    speedTram,          // Скоростной трамвай
    funicular,          // Фуникулер
    electroTrain,       // Электропоезд
    otherTransport      // Другой транспорт
};

struct SubrouteInfo_t;


//! Информация о маршруте
struct RouteInfo_t // autoinform.routes
{
    //! Номер маршрута для языка поумолчанию
    QString routeNumber;
    //! Название маршрута для языка поумолчанию
    QString routeName;
    //! Полное название маршрута для языка поумолчанию
    QString routeFullName;
    //! Тип маршрута
    InformatorRouteType_t routeType;
    //! Идентификатор маршрута
    qint64 idRoute;
    QVector<SubrouteInfo_t> subroutes;
    LedSettings_t ledPanelSettings;
    Resources_t resources;

    RouteInfo_t()
        : routeType(InformatorRouteType_t::bus)
    {}
};

//! Тип остановки
enum class StationType_t: uint8_t
{
    //! Начальная
    initial,
    //! ПромежуточнаяcontrolPoints
    mediate,
    //! Конечная
    finite,
    //! точка звукового сообщения
    message,
    unknownType
};


//! Тип обнаруживаемой информатором сущности
enum class DetectInstanceType_t
{
    //! Остановка
    Station
};


struct StationControlPoint_t
{
    //! Курс ворот, °
    double lat;
    //! Ширина ворот, °
    double lon;

    StationControlPoint_t(double _lat = 0.0, double _lon = 0.0):
            lat(_lat),
            lon(_lon)
    {}

    StationControlPoint_t(const StationControlPoint_t& other)
        : lat(other.lat),
          lon(other.lon)
    {}

    StationControlPoint_t& operator = (const StationControlPoint_t& other)
    {
        if (this != &other)
        {
            StationControlPoint_t tmp(other);

            std::swap(lat, tmp.lat);
            std::swap(lon, tmp.lon);
        }
        return *this;
    }

    ~StationControlPoint_t() = default;

    bool operator==(const StationControlPoint_t& other) const
    {
        if(!isEqalDoubles(lat, other.lat)) return false;
        if(!isEqalDoubles(lon, other.lon)) return false;
        return true;
    }

    bool operator!=(const StationControlPoint_t& other) const
    {
        return !operator==(other);
    }

private:
    static bool isEqalDoubles(double one, double two)
    {
        if(one != one && two != two) { // Оба значения nan
            return true;
        }
        else {
            return one==two;
        }
    }
};


//! Информация об остановке autoInform.routes.trips.stations
struct StationInfo_t
{
    //! Наименование остановки
    QString stationName;
    //! Широта
    double lat;
    //! Долгота
    double lon;
    //! Массив допустимых ворот захода на остановку
    QVector<StationControlPoint_t> controlPoints;
    //! Внутренний радиус остановки
    double radius;
    StationType_t stationType;
    QString audioFileName;
    QString idStation;
    QString sorting;
    AResources_t resources;

    StationInfo_t():
            lat(0.0),
            lon(0.0),
            radius(0.0),
            stationType(StationType_t::unknownType)
    {}

    StationInfo_t(const StationInfo_t& other)
        : stationName(other.stationName),
          lat(other.lat),
          lon(other.lon),
          controlPoints(other.controlPoints),
          radius(other.radius),
          stationType(other.stationType),
          audioFileName(other.audioFileName),
          idStation(other.idStation),
          sorting(other.sorting),
          resources(other.resources)
    {}

    StationInfo_t& operator = (const StationInfo_t& other)
    {
        if (this != &other)
        {
            StationInfo_t tmp(other);

            std::swap(stationName,tmp.stationName);
            std::swap(lat,tmp.lat);
            std::swap(lon,tmp.lon);
            std::swap(controlPoints,tmp.controlPoints);
            std::swap(radius,tmp.radius);
            std::swap(stationType,tmp.stationType);
            std::swap(audioFileName,tmp.audioFileName);
            std::swap(idStation,tmp.idStation);
            std::swap(sorting,tmp.sorting);
            std::swap(resources, tmp.resources);
        }
        return *this;
    }

    ~StationInfo_t() = default;

    bool operator==(const StationInfo_t &other) const
    {
        if (stationName != other.stationName) return false;
        if (!isEqalDoubles(lat, other.lat)) return false;
        if (!isEqalDoubles(lon, other.lon)) return false;
        if (controlPoints != other.controlPoints) return false;
        if (!isEqalDoubles(radius, other.radius)) return false;
        if (stationType != other.stationType) return false;
        if (audioFileName!=other.stationName) return false;
        if (resources != other.resources) return false;
        if (idStation != other.idStation) return false;
        if (sorting != other.sorting) return false;
        return true;
    }

    bool operator!=(const StationInfo_t &other) const
    {
        return !operator==(other);
    }

private:
    static bool isEqalDoubles(double one, double two)
    {
        if(one != one && two != two) { // Оба значения nan
            return true;
        }
        else
        {
            return one == two;
        }
    }
};

//! Расширенная информация об остановке
struct ExtandedStationInfo_t
{
    StationInfo_t mainInfo;
    //! Дата и время прибытия на остановку (UTC)
    QDateTime arriveTime;
    //! Время пребывания на остановке, с
    int32_t stationDuration;

    bool operator==(const ExtandedStationInfo_t& other) const
    {
        if (mainInfo != other.mainInfo) return false;
        if (arriveTime != other.arriveTime) return false;
        if (stationDuration != other.stationDuration) return false;
        return true;
    }

    bool operator !=(const ExtandedStationInfo_t& other) const
    {
        return !operator==(other);

    }
};

//! Информация о подмаршруте
struct SubRouteStateInfo_t
{
    QString subrouteName;
    //! Список остановок
    QVector<ExtandedStationInfo_t> stations;

    bool operator==(const SubRouteStateInfo_t& other) const
    {
        if (subrouteName != other.subrouteName) return false;
        if (stations != other.stations) return false;
        return true;
    }

    bool operator !=(const SubRouteStateInfo_t& other) const
    {
        return !operator==(other);
    }
};

enum TripDirection
{
    Straight,
    Reverse,
    Round,
    UnKnown
};

//! Информация о подмаршруте autoinform.routes.trips
struct SubrouteInfo_t
{
    QString subrouteName; // trip (new)
    //! Путь аудиофайла названия подмаршрута, пока не используется
    QString subrouteSoundPath;
    //! Список остановок
    QVector<StationInfo_t> stations;
    qint64 idTrip;
    TripDirection tripType;
    Resources_t resources;
    //! Трек рейса, как массив блоков координат широта/долгота
    QVector<QVector<double>> roadTrack;
    //! Краткое описание рейса
    QString shortDescription;

    SubrouteInfo_t()
    {}

    SubrouteInfo_t(const SubrouteInfo_t& other)
        : subrouteName(other.subrouteName),
          subrouteSoundPath(other.subrouteSoundPath),
          stations(other.stations),
          idTrip(other.idTrip),
          tripType(other.tripType),
          resources(other.resources),
          roadTrack(other.roadTrack),
          shortDescription(other.shortDescription)
    {}

    SubrouteInfo_t& operator = (const SubrouteInfo_t& other)
    {
        if (this != &other)
        {
            SubrouteInfo_t tmp(other);

            subrouteName.swap(tmp.subrouteName);
            subrouteSoundPath.swap(tmp.subrouteSoundPath);
            stations.swap(tmp.stations);
            idTrip = tmp.idTrip;
            tripType = tmp.tripType;
            resources = tmp.resources;
            roadTrack = tmp.roadTrack;
            shortDescription = tmp.shortDescription;
        }
        return *this;
    }

    bool operator==(const SubrouteInfo_t& other) const
    {
        if (subrouteName != other.subrouteName) return false;
        if (stations != other.stations) return false;
        if (idTrip != other.idTrip) return false;
        if (tripType != other.tripType) return false;
        if (resources != other.resources) return false;
        if (roadTrack != other.roadTrack) return false;
        if (shortDescription != other.shortDescription) return false;
        return true;
    }

    bool operator !=(const SubrouteInfo_t& other) const
    {
        return !operator==(other);
    }
};

//! Состояние маршрута
struct RouteState_t
{
    int32_t routeIndex;
    int16_t subrouteIndex;
    int16_t stationIndex;
    int32_t nextStationIndex;
    //! Пользовательский номер станции
    int16_t userStationIndex;
    //! Пользовательский номер следующей станции
    int16_t userNextStationIndex;
    bool isLocatedOnStation;
    float subroutePercentage;
    //! Перечень подмаршрутов
    QVector<SubRouteStateInfo_t> subRoutes;

    bool operator==(const RouteState_t& other) const
       {
           if (routeIndex != other.routeIndex) return false;
           if (subrouteIndex != other.subrouteIndex) return false;
           if (stationIndex != other.stationIndex) return false;
           if (nextStationIndex != other.nextStationIndex) return false;
           if (userStationIndex != other.userStationIndex) return false;
           if (userNextStationIndex != other.userNextStationIndex) return false;
           if (isLocatedOnStation != other.isLocatedOnStation) return false;
           if (subroutePercentage != subroutePercentage) return  false;
           if (subRoutes != other.subRoutes) return false;

           return true;
       }

       bool operator!=(const RouteState_t& other) const
       {
           return !operator==(other);
       }
};

//! Тип скролинга на лед-панелях
enum class ELedPanelScrolingType: uint8_t
{
    //! Вертикальный скролинг
    vertical,
    //! Горизонтальный скролинг по одной остановке
    horisontalOneStation,
    //! Горизонтальный скролинг списком
    horisontalAsList
};

//! Расположение рисунка на led-панели
enum class ELedPanelImageLocation
{
    dontShow,               // 1) Не отображать
    replaceRouteNumber,     // 2) Вместо номера маршрута
    beforeRouteNumber,      // 3) Перед номером маршрута
    afterNumber             // 4) За номером маршрута
};

//! Информация об остановки для других языков
struct OtherLanguageStationInfo_t
{
    QString stationName;    // наименование остановки
    QString audioFileName;  // Путь к аудиофайлу остановки
    QString additionalInfo; // Дополнительная информация остановки.
};

// Данные маршрута для других языков (не по умолчанию)
// Если некоторые поля не заполнены, то строка будет взята из соответствующей
// строки по умолчанию.
struct OtherLanguagesSubroute_t
{
    QString subrouteName;
    QVector<OtherLanguageStationInfo_t> stations; // Список остановок
};

struct OtherLanguagesData_t
{
    QString languageName;   // Название языка
    QString routeNumber;    // Номер маршрута
    QString routeName;      // Название – короткое текстовое название остановки. Служит для
                            // визуальной идентификации остановки во время подготовки информации, а также
                            // первые 15 символов названия будут отображаться в информаторе с дисплеем во
                            // время выбора водителем остановки из списка. К этому названию добавляется
                            // первый символ имени фразы из шаблона фраз остановки. Названия начальной,
                            // конечной и текущей остановок для табло берутся из этого поля.
    QString routeFullName;  // Полное название – полное текстовое название остановки без сокращений. Это
                            // название остановки выводится на внутреннем табло. Если название не помещается
                            // на табло, то текст названия выводится в виде бегущей строки. Если табло не
                            // используется, то это поле можно не заполнять. Если используется табло, а это поле
                            // пустое, то применяется короткое название остановки.

    QString routeSoundPath;         // Путь аудиофайла названия маршрута
    QVector<OtherLanguagesSubroute_t> subroutes;
};

struct InformatorSettings_t
{
    int32_t watchdogTimeout;
    //! малый радиус обнаружения остановки, м
    float stationRadius;
    //! Гистерезис, при выходе за пределы малого радиуса плюс гистерезиса
    float stationHisteresis;
    //! Путь к файлу маршрутной сети
    QString routesFilePath;
};


// TODO: данные типы скопипастены взяты из библиоеки nmea *********
enum class EFix_type_informator: uint8_t
{
  INACTIVE = 0,
  NOT_AVALIABLE  = 1,
  FIX2D = 2,
  FIX3D = 3,
};

enum class ECoord_status_informator: uint8_t
{
  INVALID = 0,
  VALID  = 1
};

struct time_struct_informator_t
{       /* date and time components */
    uint8_t             sec;
    uint8_t             min;
    uint8_t             hour;
    uint8_t             day;
    uint8_t             mon;
    uint16_t        year;
    time_struct_informator_t():
        sec(0),
        min(0),
        hour(0),
        day(0),
        mon(0),
        year(0)
    {}
};

struct GnssState_t
{
    float    alt;
    float    direction;
    EFix_type_informator fix_type;
    float   HDOP;
    ECoord_status_informator coord_status;
    // Широта (знаковая. градусы, доли градуса. )
    double    lat;
    // Долгота (знаковая. градусы, доли градуса. )
    double    lon;
    // Position Dilution Of Precision
    float   PDOP;
    // Количество спутников, используемых в навигационном решении
    uint8_t sat_inuse;
    uint8_t total_sat_inview;
    // Скорость
    float    speed;
    time_struct_informator_t utc;
    // Vertical Dilution Of Precision
    float   VDOP;
};


//! Обнаруженные "прибытия на"/"убытия с" остановок
struct DetectedStation_t
{
    //! Индекс подмаршрута
    int32_t subrouteIndex;
    int32_t stationIndex;
};

inline bool operator==(const DetectedStation_t& one, const DetectedStation_t& two)
{
    return one.stationIndex == two.stationIndex &&
           one.subrouteIndex == two.subrouteIndex;
}

//! Статус обнаружения остановки
enum class StationDetectStatus_t: uint8_t
{
    Arrived,
    Left
};

struct RouteFullInfo_t
{
    RouteInfo_t title;
    //! автопарк
    QString idDepot;
    //! название автопарка
    QString depotName;
    //! перевозчик
    QString idAgency;
};
