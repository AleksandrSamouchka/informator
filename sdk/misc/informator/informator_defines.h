#ifndef INFORMATOR_DEFINES_H
#define INFORMATOR_DEFINES_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QDateTime>

// Ошибки информатора
enum class InformatorError_t: uint8_t
{
    noError,
    wrongRouteIndexError, // Указан неверный индекс маршрута
    wrongSubrouteIndexError // Указан неверный индекс подмаршрута
};


// Режим работы информатора
enum class InformatorWorkMode_t : uint8_t
{
    Auto,
    Manual,
    AutoAutoSubroute,
    Unknown
};


// Группы динамиков звукового оповещения
enum class SoundInfoType_t : uint8_t
{
    INSIDE_INFORMER_GROUP,
    OUTSIDE_INFORMER_GROUP,
    unknownValue            // Неизвестное значение
};


// Подмаршрут (направление движения туда/обратно)
/*enum class SubRoute_t : uint8_t
{
    first,
    second
};*/


// Режим работы информатора
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

// Информация о маршруте
struct RouteInfo_t
{
    QString routeNumber; // Номер маршрута для языка поумолчанию
    QString routeName;  // Название маршрута для языка поумолчанию
    QString routeFullName; // Полное название маршрута для языка поумолчанию
    InformatorRouteType_t routeType; // Тип маршрута
    QVector<SubrouteInfo_t> subroutes;
    //QVector<StandardWord_t> stWords; // Стандартные слова для шаблонов
    //QVector<Template_t> templates; // шаблоны, используемые в маршруте
    RouteInfo_t():
            routeType(InformatorRouteType_t::bus)
    {}

    bool operator==(const RouteInfo_t &other) const
    {
        if(routeNumber!= other.routeNumber) return false;
        if(routeName!= other.routeName) return false;
        if(routeFullName!= other.routeFullName) return false;
        if(routeType != other.routeType) return false;
        if(subroutes != other.subroutes) return false;
        return true;
    }
    bool operator!=(const RouteInfo_t &other) const
    {
        return !operator==(other);
    }
};


// Тип остановки
enum class StationType_t: uint8_t
{
    initial,      // Начальная
    mediate,      // Промежуточная
    finite,       // Конечная
    message,      // точка звукового сообщения
    unknownType
};


// Тип обнаруживаемой информатором сущности
enum class DetectInstanceType_t
{
    Station,                // Остановка
    //ControlPoint          // Контрольная точка (пока что не будем ее обнаруживать)
    VoiceMessagePoint       // Точка координат голосового сообщения
};


struct StationControlPoint_t
{
    double lat; // Курс ворот, °
    double lon; // Ширина ворот, °

    StationControlPoint_t(double _lat = 0.0, double _lon = 0.0):
            lat(_lat),
            lon(_lon)
    {}

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


// Информация об остановке
struct StationInfo_t
{
    QString stationName;   // наименование остановки
    double lat;     // Широта
    double lon;     // Долгота
    //double course;  // Курс захода на остановку, °
    QVector<StationControlPoint_t> controlPoints; // Массив допустимых ворот захода на остановку
    double radius;  // Внутренний радиус остановки
    StationType_t stationType;
    QString audioFileName;
    QString additionalInfo;

    StationInfo_t():
            lat(0.0),
            lon(0.0),
            radius(0.0),
            stationType(StationType_t::unknownType)
    {}

    bool operator==(const StationInfo_t &other) const
    {
        if(stationName != other.stationName) return false;
        if(!isEqalDoubles(lat, other.lat)) return false;
        if(!isEqalDoubles(lon, other.lon)) return false;
        if(controlPoints != other.controlPoints) return false;
        if(!isEqalDoubles(radius, other.radius)) return false;
        if(stationType != other.stationType) return false;
        if(audioFileName!=other.stationName) return false;
        if(additionalInfo!=other.additionalInfo) return false;
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
        else {
            return one==two;
        }
    }
};

// Расширенная информация об остановке
struct ExtandedStationInfo_t
{
    StationInfo_t mainInfo;
    //int16_t stationIndex;          // номер остановки
    QDateTime ariveTime;            // Дата и время прибытия на остановку (UTC)
    int32_t stationDuration;       // Время пребывания на остановке, с
};

// Информация о подмаршруте
struct SubRouteStateInfo_t
{
    QString subrouteName;
    QVector<ExtandedStationInfo_t> stations; // Список остановок
};

// Расписание одной поездки (trip)
/*struct  scheduleTrip_t
{
    QVector<QTime>

};*/

// Информация о подмаршруте
struct SubrouteInfo_t
{
    QString subrouteName;
    QString subrouteSoundPath;         // Путь аудиофайла названия подмаршрута
        //                                // для языка по умолчанию.
    QVector<StationInfo_t> stations; // Список остановок
    QVector<StationInfo_t> voicePoints; // Список точек голосовых сообщений
    // QVector<scheduleTrip_t>  trips;    // Расписание поездок

    bool operator==(const SubrouteInfo_t& other) const
    {
        if(subrouteName != other.subrouteName) return false;
        if(stations != other.stations) return false;
        if(voicePoints != other.voicePoints) return false;
        return true;
    }
    bool operator !=(const SubrouteInfo_t& other) const
    {
        return !operator==(other);
    }
};


// Состояние маршрута
struct RouteState_t
{
    int32_t routeIndex;
    int16_t subrouteIndex;
    int16_t stationIndex;
    int32_t nextStationIndex;
    int16_t userStationIndex;     // Пользовательский номер станции ()
    int16_t userNextStationIndex; // Пользовательский номер следующей станции
    bool isLocatedOnStation;
    float subroutePercentage;
    QVector<SubRouteStateInfo_t> subRoutes; // Перечень подмаршрутов
};

// Информация о маршруте
/*struct RouteInfo_t
{
    int32_t routeNumber;
    int16_t currentSubRoute;
    int32_t nextStationIndex;
    float subroutePercentage;
    QVector<SubRouteStateInfo_t> subRoutes; // Перечень подмаршрутов
};*/


// Тип скролинга на лед-панелях
enum class ELedPanelScrolingType: uint8_t
{
    vertical,               // 1) Вертикальный скролинг,
    horisontalOneStation,   // 2) Горизонтальный скролинг по одной остановке
    horisontalAsList        // 3) Горизонтальный скролинг списком
};

// Расположение рисунка на led-панели
enum class ELedPanelImageLocation
{
    dontShow,               // 1) Не отображать
    replaceRouteNumber,     // 2) Вместо номера маршрута
    beforeRouteNumber,      // 3) Перед номером маршрута
    afterNumber             // 4) За номером маршрута
};



// Информация об остановки для других языков
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
    QVector<OtherLanguageStationInfo_t> voicePoints; // Список точек голосовых сообщений
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
    //QString routeEnding1SoundPath;  //[deprecated] Путь аудиофайла названия конечной остановки 1 маршрута
    //QString routeEnding2SoundPath;  // [deprecated] Путь аудиофайла названия конечной остановки 2 маршрута
    QVector<OtherLanguagesSubroute_t> subroutes;
};


// Настройки led-панелей
struct LedPanelsSettings
{
    bool enableApplySettings;    // Разрешить применение параметров табло для маршрута
    ELedPanelScrolingType scrolingType;   // Тип скролинга
    bool wideListStationsOutEnabled; // Список промежуточных остановок выводить широким списком
    bool allStationsToOneString; // Начальную, конечную, промежуточные остановки выводить одной строкой.
    bool sidePanelLikeFrontOne; // Боковое табло как переднее для маршрута
    bool frontPanelLikeSideOne; // Боковое табло как переднее для маршрута
    bool startAndEndStationsToFrontPanel; // Выводить на переднее табло "<Начальная> >> <Конечная>".
    bool endStationsOnlyToFrontPanelAsRunningString; // Выводить на переднее табло только название конечной остановки бегущей строкой
    bool endStationsOnlyToFrontPanelAsStaticText; // Выводить на переднее табло только название конечной остановки статически
    bool fullNamesAndAdverToFrondPanel; // Выводить полные названия остановок и рекламу на переднюю панель
    bool fullNamesAndAdverToSidePanel; // Выводить полные названия остановок и рекламу на боковую панель
    bool timeRenderDisable; // Запретить выводить на табло текущее время.
    bool insideTemperRenderDisable; // Запретить выводить на табло температуру в салоне
    bool disableRouteNumberToFrontPanel; // Запретить выводить номер маршрута на переднюю панель
    bool disableeRouteNumberToSidePanel; // Запретить выводить номер маршрута на боковую панель
    QString imagePath;
    ELedPanelImageLocation imageLocationFrontPanel;
    ELedPanelImageLocation imageLocationSidePanel;
    ELedPanelImageLocation imageLocationBackPanel;

    LedPanelsSettings():
            enableApplySettings(false),
            scrolingType(ELedPanelScrolingType::horisontalAsList),
            wideListStationsOutEnabled(false),
            allStationsToOneString(false),
            sidePanelLikeFrontOne(false),
            frontPanelLikeSideOne(false),
            startAndEndStationsToFrontPanel(false),
            endStationsOnlyToFrontPanelAsRunningString(false),
            endStationsOnlyToFrontPanelAsStaticText(false),
            fullNamesAndAdverToFrondPanel(false),
            fullNamesAndAdverToSidePanel(false),
            timeRenderDisable(false),
            insideTemperRenderDisable(false),
            disableRouteNumberToFrontPanel(false),
            disableeRouteNumberToSidePanel(false),
            imageLocationFrontPanel(ELedPanelImageLocation::dontShow),
            imageLocationSidePanel(ELedPanelImageLocation::dontShow),
            imageLocationBackPanel(ELedPanelImageLocation::dontShow)
    {}
    // ????? Добавить еще цвета шрифтов и фона для каждой из led-панелей.
};




struct InformatorSettings_t {
//    QString dbIp;
//    uint16_t dbPort;
//    QString dbUser;
//    QString dbPasw;
    QString mqttIp;
    uint16_t mqttPort;
    QString mqttIntegrationIp;
    uint16_t mqttIntegrationPort;
    uint8_t mqttIntegrationQos;                  // mqttIntegrationQos
    QString routesFilePath;
    int32_t watchdogTimeout;
    float stationRadius;        // малый радиус обнаружения остановки, м
    float stationHisteresis;    // Гистерезис, при выходе за пределы малого радиуса плюс гистерезиса
                                // считаем, что мы выехали из зоны действия остановки.
/*    QList<SoundInfoType_t> out7_1_FL;
    QList<SoundInfoType_t> out7_1_FR;
    QList<SoundInfoType_t> out7_1_CN;
    QList<SoundInfoType_t> out7_1_SW;
    QList<SoundInfoType_t> out7_1_BL;
    QList<SoundInfoType_t> out7_1_BR;
    QList<SoundInfoType_t> out7_1_SL;
    QList<SoundInfoType_t> out7_1_SR;
*/
};


// TODO: Начало говнокода: данные типы скопипастены взяты из библиоеки nmea *********
enum class EFix_type_informator: uint8_t
{
  INACTIVE = 0,
  NOT_AVALIABLE  = 1,
  FIX2D = 2,
  FIX3D = 3,
} ;


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

// Конец говнокода ************************************************************



struct GnssState_t
{
    float    alt;
    float    direction;
    EFix_type_informator fix_type;
    float   HDOP;
    ECoord_status_informator coord_status;
    double    lat;                // Широта (знаковая. градусы, доли градуса. )
    double    lon;                // Долгота (знаковая. градусы, доли градуса. )
    float   PDOP;               // Position Dilution Of Precision
    uint8_t sat_inuse;          // Количество спутников, используемых в навигационном решении
    uint8_t total_sat_inview;
    float    speed;              // Скорость
    time_struct_informator_t utc;
    float   VDOP;               // Vertical Dilution Of Precision
};


// Обнаруженные "прибытия на"/"убытия с" остановок
struct DetectedStation_t
{
    int32_t subrouteIndex; // Индекс подмаршрута
    int32_t stationIndex;
};

bool operator==(DetectedStation_t one, DetectedStation_t two);



// Статус обнаружения остановки
enum class StationDetectStatus_t: uint8_t
{
    Arived,
    Left
};



#endif // INFORMATOR_DEFINES_H
