#ifndef CINFORMATORMISC_H
#define CINFORMATORMISC_H

// Qt
#include <QObject>
#include <QDateTime>
#include <QVariant>
#include <QString>
#include <QByteArray>

//SDK
#include <Mqtt/Topics/Informator/InformatorDefines.h>

/** ***********************************************************************
 * @class CInformatorMisc
 * @brief Статический класс с различными повсеместно используемыми вспомога-
 * тельными функциями для информатора
 * @todo
 *
 ** ***********************************************************************/
class CInformatorMisc
{
public:
    CInformatorMisc() = delete;
    static QVector<RouteInfo_t> routeFullInfosToRouteInfos(const QVector<RouteFullInfo_t> &fullInfos);
    static QDateTime utcToDateTime(const time_struct_informator_t &utc);
    static int nextStation(const RouteState_t &routeState, int curStation);
    static GnssState_t gnssIncorrectState();
    static QDateTime getCorrectTime(const GnssState_t &gnssState);
    static bool additionalGnssDataVerify(const GnssState_t &gnssState);
    static QString gnssDataToString(const GnssState_t &gnssState);
    static bool verifyLatitude(double latitude);
    static bool verifyLongitude(double latitude);
    static QVariant readValueAndReport(const QString &key, const QVariant &defaultValue, QVariant::Type type = QVariant::String);
    static QString informatorGuidDescription(InformatorError_t err);
    static InformatorSettings_t *defaultSettings()
    {
        return &DEFAULT_INF_SETTINGS;
    }
private:
    static const time_struct_informator_t defaultUtc;
    static InformatorSettings_t DEFAULT_INF_SETTINGS;
};

#endif // CINFORMATORMISC_H
