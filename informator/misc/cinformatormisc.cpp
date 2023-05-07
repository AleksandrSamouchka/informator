// STL
#include <cmath>

// SDK
#include "CSettings/CSettings.h"
#include <CLoggerHandler/CLoggerHandler.h>

// Local
#include "cinformatormisc.h"

const time_struct_informator_t CInformatorMisc::defaultUtc;

InformatorSettings_t CInformatorMisc::DEFAULT_INF_SETTINGS =
{
    // watchdogTimeout - 3c
    3000,
    // StationRadius - 30 м
    30.0f,
    // stationHisteresis
    70.0f,
    ""
};

QVector<RouteInfo_t> CInformatorMisc::routeFullInfosToRouteInfos(const QVector<RouteFullInfo_t> &fullInfos)
{
    QVector<RouteInfo_t> infos;
    infos.reserve(fullInfos.size());
    for(const RouteFullInfo_t &fullInfo: fullInfos)
    {
        infos.append(fullInfo.title);
    }
    return infos;
}

QDateTime CInformatorMisc::utcToDateTime(const time_struct_informator_t &utc)
{
    QDateTime dt;
    QDate date;
    QTime time;
    date.setDate(utc.year, utc.mon, utc.day);
    time.setHMS(utc.hour, utc.min, utc.sec);
    dt.setDate(date);
    dt.setTime(time);
    return dt;
}

/** ****************************************************************************
 * @fn nextStation
 * @brief // Определяем следующую остановку
 * @param int  curStation - текущая остановка
 * @return int - следующая остановка
 ** ***************************************************************************/
int CInformatorMisc::nextStation(const RouteState_t &routeState,
                                       int curStation)
{
    //if(routeState.routeIndex<0 ||
    //        routeState.routeIndex >= m_routes.size())
    //    return -1;
    auto &subroutes = routeState.subRoutes;
    if(routeState.subrouteIndex<0 ||
            routeState.subrouteIndex >= subroutes.size())
        return -1;
    auto &stations = subroutes[routeState.subrouteIndex].stations;
    if(curStation<0 ||
            curStation >= stations.size())
        return -1;
    int nextStation = ((++curStation) >= stations.size())? -1: curStation;
    return nextStation;
}


/** ****************************************************************************
 * @fn gnssIncorrectState
 * @brief Возвращает структуру с флагом неверных данных от gnss
 * @return структура с флагом о том, что координаты и время от gnss не верны
 ** ***************************************************************************/
GnssState_t CInformatorMisc::gnssIncorrectState()
{
    GnssState_t state = {
        NAN,                         //  float    alt;
        NAN,                        //  float    direction;
        EFix_type_informator::NOT_AVALIABLE,   //  EFix_type fix_type;
        NAN,                            //  float   HDOP;
        ECoord_status_informator::INVALID,                             //  ECoord_status coord_status;
        NAN,                            //  double    lat;                // Широта (знаковая. градусы, доли градуса. )
        NAN,                            //  double    lon;                // Долгота (знаковая. градусы, доли градуса. )
        NAN,                            //  float   PDOP;               // Position Dilution Of Precision
        0,                            //  uint8_t sat_inuse;          // Количество спутников, используемых в навигационном решении
        0,                             //  uint8_t total_sat_inview;
        NAN,                            //  float    speed;              // Скорость
        defaultUtc,                            //  time_struct_t utc;
        NAN                            //  float   VDOP;
    };

    //state.alt = -1.0;
    //state.coord_status = ECoord_status::INVALID;
    return state;
}


/** ****************************************************************************
 * @fn getCorrectTime
 * @brief Возвращает корректное время. (использует время gnss, если есть связь
 * со спутниками, в противном случае использует системное время)
 * @return возвращает системное время
 ** ***************************************************************************/
QDateTime CInformatorMisc::getCorrectTime(const GnssState_t &gnssState)
{
    if(gnssState.coord_status!= ECoord_status_informator::VALID) { // Если данные gnss не верные,
                                                    // то отдаем системное время
        return QDateTime::currentDateTime();
    }
    // Иначе, отдаем время gnss
    return CInformatorMisc::utcToDateTime(gnssState.utc);
}


/** ****************************************************************************
 * @fn additionalGnssDataVerify
 * @brief Дополнительная проверка данных GNSS. Если координаты нулевые,
 * то считаем, что данные гнсс некоректные
 * @return true - данные корректные
 *          false - данные некорректные
 ** ***************************************************************************/
bool CInformatorMisc::additionalGnssDataVerify(const GnssState_t &gnssState)
{
    if(gnssState.coord_status == ECoord_status_informator::INVALID) return false;
    if(gnssState.lat == 0.0 &&
            gnssState.lon == 0.0)
        return false;
    if(!verifyLatitude(gnssState.lat)) return false;
    if(!verifyLongitude(gnssState.lon)) return false;
    return true;
}


/** ****************************************************************************
 * @fn gnssDataToString
 * @brief Преобразует данные GNSS в строку для удобного просмотра.
 * @param GnssState_t gnssState - данные gnss
 * @return QString - Данные GNSS в виде строки
 ** ***************************************************************************/
QString CInformatorMisc::gnssDataToString(const GnssState_t &gnssState)
{
    QDate date(gnssState.utc.year, gnssState.utc.mon, gnssState.utc.day);
    QTime time(gnssState.utc.hour, gnssState.utc.min, gnssState.utc.sec);
    QString coordStatus = (gnssState.coord_status == ECoord_status_informator::INVALID)?
                "INVALID" : "VALID";
    QString fixType;
    switch(gnssState.fix_type) {
    case EFix_type_informator::INACTIVE: fixType = "INACTIVE"; break;
    case EFix_type_informator::NOT_AVALIABLE: fixType = "NOT_AVALIABLE"; break;
    case EFix_type_informator::FIX2D: fixType = "FIX2D"; break;
    case EFix_type_informator::FIX3D: fixType = "FIX3D"; break;
    default: fixType = "NOT_AVALIABLE";
    }

    QString str = QString("alt = %1, direction = %2, fix_type = %3, HDOP = %4, "
            "coord_status = %5, lat = %6, lon = %7, PDOP = %8, "
            "sat_inuse = %9, total_sat_inview = %10, speed = %11, Date = %12, "
            "time = %13, VDOP = %14").
            arg(gnssState.alt).
            arg(gnssState.direction).
            arg(fixType).
            arg(gnssState.HDOP).
            arg(coordStatus).
            arg(gnssState.lat).
            arg(gnssState.lon).
            arg(gnssState.PDOP).
            arg(gnssState.sat_inuse).
            arg(gnssState.total_sat_inview).
            arg(gnssState.speed).
            arg(date.toString()).
            arg(time.toString()).
            arg(gnssState.VDOP);
    return str;
}


/** ****************************************************************************
 * @fn verifyLatitude
 * @brief Проверяет корректность заданной широты
 * @param double latitude - широта
 * @return true - значение широты корректное
 *          false  - значение широты ошибочное
 ** ***************************************************************************/
bool CInformatorMisc::verifyLatitude(double latitude)
{
    if(latitude < -90.0 || latitude >90.0) return false;
    else if(latitude != latitude) return false; // Nan
    else return true;
}

/** ****************************************************************************
 * @fn verifyLongitude
 * @brief Проверяет корректность заданной широты
 * @param double latitude - широта
 * @return true - значение широты корректное
 *          false  - значение широты ошибочное
 ** ***************************************************************************/
bool CInformatorMisc::verifyLongitude(double latitude)
{
    if(latitude < -180.0 || latitude >180.0) return false;
    else if(latitude != latitude) return false; // Nan
    else return true;
}

/** ***********************************************************************
 * @fn readValueAndReport [static]
 * @brief метод для чтения параметра из файла настроек. В случае
 * возникновения ошибки чтения настроек выводит сообщение в консоль
 * и в лог GUI
 * @param QString - ключ для поиска
 * @param QVariant - значение по умолчанию (если параметр не найден)
 * @return QVariant - значение считанной настройки
 ** ***********************************************************************/
QVariant CInformatorMisc::readValueAndReport(const QString &key, const QVariant &defaultValue, QVariant::Type type)
{
    CSettingsErrors_t error = CSettingsErrors_t::noError;
    QVariant result = CSettings::readValue( key, error, defaultValue );
    switch ( error ) {
    case CSettingsErrors_t::fileOpenError:
        PRINT_LOG_MESSAGE_WARN( QString( "Can't open settings file: %1, "
                                          "using default value: %2 for key: %3" ).
                                 arg(CSettings::fileName()).
                                 arg(defaultValue.toString()).
                                 arg(key));
        break;
    case CSettingsErrors_t::fieldReadError:
        PRINT_LOG_MESSAGE_WARN(QString( "Can't find setting %1 within "
                                         "file %2, using default value: %3" ).
                                arg(key).
                                arg(CSettings::fileName()).
                                arg(defaultValue.toString()));
        break;
    case CSettingsErrors_t::fieldHasWrongValueError: // это значение
        break;                                      // будет использоваться ниже
        // На эту метку программа не должна попасть.
    case CSettingsErrors_t::noError:
        break;
    }
    if(type == QVariant::Int) // Не можем конвертировать в заданный тип
    {
        bool ok;
        result.toInt(&ok);
        if(!ok) {
            PRINT_LOG_MESSAGE_WARN( QString( "Wrong value %1 setting's parameter"
                                             " %2 . Will be used default setting value %3").
                                    arg(result.toString()).
                                    arg(key).
                                    arg(defaultValue.toString()));
            return defaultValue;
        }
    }
    else if(type == QVariant::Double) {
        bool ok;
        result.toDouble(&ok);
        if(!ok) {
            PRINT_LOG_MESSAGE_WARN( QString( "Wrong value %1 setting's parameter"
                                             " %2 . Will be used default setting value %3").
                                    arg(result.toString()).
                                    arg(key).
                                    arg(defaultValue.toString()));
            return defaultValue;
        }
    }
    return result;
}

QString CInformatorMisc::informatorGuidDescription(InformatorError_t err)
{
    switch(err) {
    case InformatorError_t::wrongRouteIndexError:
        return  "Задан неверный индекс маршрута";
    case InformatorError_t::wrongSubrouteIndexError:
        return "Задан неверный индекс подмаршрута";
    default:
        return "Неизвестная ошибка гида информатора";
    }
}
