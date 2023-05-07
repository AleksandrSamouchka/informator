#include <misc/informator/cinformatormisc.h>
#include <QLinkedList>
#include <QRegExp>
#include <cmath>
#include <QCryptographicHash>
#include <QFile>
#include "CSettings/CSettings.h"
#include <CLoggerHandler/CLoggerHandler.h>

const time_struct_informator_t CInformatorMisc::defaultUtc;
/*=
{
    0,       // uint8_t             sec;
    0,       // uint8_t             min;
    0,        // uint8_t             hour;
    0,       // uint8_t             day;
    0,       // uint8_t             mon;
    0       // uint16_t        year;
};*/

InformatorSettings_t CInformatorMisc::DEFAULT_INF_SETTINGS =
{
//    QStringLiteral("127.0.0.1"),    // dbIp
//    3306,           // dbPort
//    QStringLiteral("mntconti"),        // dbUser
//    QStringLiteral("18iTnOcTnM81"),           // dbPasw
    QStringLiteral("127.0.0.1"),    // mqttIp
    1883,           // mqttPort
    QStringLiteral("127.0.0.1"),    // mqttIntegtrationIp
    1883,               // mqttIntegtrationPort
    2,                  // mqttIntegrationQos
    QStringLiteral("routes.bus"),       // RoutesFilePath
    3000,                           // watchdogTimeout - 3c
    30.0f,                            // StationRadius - 30 м
    70.0f            // stationHisteresis
//    {}, // out7_1_FL
//    {SoundInfoType_t::INSIDE_INFORMER_GROUP}, // out7_1_FR
//    {}, //SoundInfoType_t::INSIDE_INFORMER_GROUP, SoundInfoType_t::OUTSIDE_INFORMER_GROUP}, // out7_1_CN
//    {}, //SoundInfoType_t::INSIDE_INFORMER_GROUP, SoundInfoType_t::OUTSIDE_INFORMER_GROUP}, // out7_1_SW
//    {}, //SoundInfoType_t::INSIDE_INFORMER_GROUP, SoundInfoType_t::OUTSIDE_INFORMER_GROUP}, // out7_1_BL
//    {}, //SoundInfoType_t::INSIDE_INFORMER_GROUP, SoundInfoType_t::OUTSIDE_INFORMER_GROUP}, // out7_1_BR
//    {}, //SoundInfoType_t::INSIDE_INFORMER_GROUP, SoundInfoType_t::OUTSIDE_INFORMER_GROUP}, // out7_1_SL
//    {SoundInfoType_t::OUTSIDE_INFORMER_GROUP} //  out7_1_SR

};

const QVector<RouteInfo_t> CInformatorMisc::routeFullInfosToRouteInfos(
        const QVector<RouteFullInfo_t> &fullInfos)
{
    QVector<RouteInfo_t> infos;
    infos.reserve(fullInfos.size());
    for(const RouteFullInfo_t &fullInfo: fullInfos) {
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


/*QString CInformatorMisc::getAudioFormatDescription(const QAudioFormat &format)
{
    QString sampleType = "Not Set";
    switch(format.sampleType()) {
    case QAudioFormat::Unknown: sampleType = "Not Set"; break;
    case QAudioFormat::SignedInt: sampleType = "Samples are signed integers"; break;
    case QAudioFormat::UnSignedInt: sampleType = "Samples are unsigned intergers"; break;
    case QAudioFormat::Float: sampleType = "Samples are floats"; break;
    }

    QString str = QString("Endian = %1\n"
                "SampleRate = %2\n"
                "ChannelNumber = %3\n"
                "SampleSize = %4\n"
                "SampleType = %5\n").
            arg((format.byteOrder()== QAudioFormat::Endian::BigEndian)?
                    QString("Big Endian"): QString("Litle endian")).
            arg(format.sampleRate()).
            arg(format.channelCount()).
            arg(format.sampleSize()).
            arg(sampleType);
    return str;
}
*/

/** ***********************************************************************
 * @fn repairStantionsAndvoiceMessages
 * @brief Перенос точек голосовых сообщений из массива станций в массив голосовых сообщений
 * @param
 ** ***********************************************************************/
void CInformatorMisc::uncoupleStantionsAndVoiceMessages(QVector<RouteFullInfo_t> &routes)
{
    for(RouteFullInfo_t &route: routes) {
        for(SubrouteInfo_t &subroute: route.title.subroutes) {
            // Формируем список точек голосовых сообщений, разыскивая их в массиве станций
            QLinkedList<StationInfo_t> newStations;
            QLinkedList<StationInfo_t> newVoicePoints;
            for(StationInfo_t &station: subroute.stations) {
                if(station.stationType == StationType_t::message) {
                    newVoicePoints << station;
                }
                else newStations << station;
            }
            subroute.voicePoints.resize(newVoicePoints.size());
            subroute.stations.resize(newStations.size());
            int i = 0;
            for(StationInfo_t &point: newVoicePoints)
                subroute.voicePoints[i++] = point;
            i = 0;
            for(StationInfo_t &station: newStations)
                subroute.stations[i++] = station;
        }
    }
}


// Заменяет встречаемые в строке точки на запятые, или наоборот.
// Используется для приведения к верной строке для перевода в значение
// с плавающей запятой
void CInformatorMisc::changePointAndComma(QString &str)
{
    int index = str.indexOf(",");
    if(index>= 0) {
        str = str.replace(index, 1, ".");
        return;
    }
    index = str.indexOf(".");
    if(index>= 0) {
        str = str.replace(index, 1, ",");
    }
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
 * @fn verifyAudioFilePath
 * @brief Проверяет путь к файлу на предмет: может ли указанный путь к файлу
 * использоваться как путь к аудиофайлу для воспроизведения голосовых сообщений
 * в ОС linux. Путь может содержать только латинские буквы, цифры, знак
 * подчеркивания. И не должен содержать символов "-", ","
 * @param const QString &path - путь к файлу
 * @param int &simbNumber - возвращаемый функцией номер символа, который не
 * удовлетворяет условию корректности пути
 * @return true - путь корректный
 *          false  - путь некорректный
 ** ***************************************************************************/
bool CInformatorMisc::verifyAudioFilePath(const QString &path, int &simbNumber)
{
    int index = path.indexOf(QRegExp("[-,]"));
    if(index != -1) {
        simbNumber = index;
        return false;
    }
    return true;
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


/** ****************************************************************************
 * @fn stringToHexLine
 * @brief Преобразует байтовый массив в его строковое hex-представление,
 * разделенную пробелами
 * @param const QByteArray &arr - входной массив
 * @return возвращает hex-строку
 ** ***************************************************************************/
QString CInformatorMisc::byteArrayToHexCodeLine(const QByteArray &arr)
{
    QString out;
    int count = arr.size();
    for(int i=0; i<count; i++) {
        uint8_t simb = arr[i];
        QString hex = QString::number(simb, 16);
        if(hex.size() == 1) out.append("0");
        out.append(hex);
        out.append(" ");
    }
    return out;
}


/** ****************************************************************************
 * @fn calcFileMd5
 * @brief Считает HASH MD5 для файла
 * @param const QByteArray &arr - входной массив
 * @return возвращает hex-строку
 ** ***************************************************************************/
bool CInformatorMisc::calcFileMd5(const QString &fileName, QByteArray &md5)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile file(fileName);
    bool ok = file.open(QFile::ReadOnly);
    if(!ok) return false;
    hash.addData(file.readAll());
    md5 = hash.result();
    return true;
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

QString CInformatorMisc::soundInfoTypeListToString(const QList<SoundInfoType_t> &list)
{
    QString res;
    for(auto &item: list) {
        if(!res.isEmpty()) res.append(",");
        switch(item) {
        case SoundInfoType_t::INSIDE_INFORMER_GROUP:
            res.append("INSIDE_INFORMER");
            break;
        case SoundInfoType_t::OUTSIDE_INFORMER_GROUP:
            res.append("OUTSIDE_INFORMER");
            break;
        default:
            res.append("UnknownValue");
        }
    }
    return res;
}




