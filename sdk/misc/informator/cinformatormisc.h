#ifndef CINFORMATORMISC_H
#define CINFORMATORMISC_H

#include <QObject>
#include <QDateTime>
#include <QVariant>
//#include <QAudioFormat>
#include <QString>
#include <misc/routes/routes.h>

#include <QByteArray>

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
    static const QVector<RouteInfo_t> routeFullInfosToRouteInfos(
            const QVector<RouteFullInfo_t> &fullInfos);
    static QDateTime utcToDateTime(const time_struct_informator_t &utc);
//    static QString getAudioFormatDescription(const QAudioFormat &format);
    static void uncoupleStantionsAndVoiceMessages(QVector<RouteFullInfo_t> &routes);
    static void changePointAndComma(QString &str);
    static int nextStation(const RouteState_t &routeState, int curStation);
    static GnssState_t gnssIncorrectState();
    static QDateTime getCorrectTime(const GnssState_t &gnssState);
    static bool additionalGnssDataVerify(const GnssState_t &gnssState);
    static QString gnssDataToString(const GnssState_t &gnssState);
    static bool verifyAudioFilePath(const QString &path, int &simbNumber);
    static bool verifyLatitude(double latitude);
    static bool verifyLongitude(double latitude);
    static QString byteArrayToHexCodeLine(const QByteArray &arr);
    static bool calcFileMd5(const QString &fileName, QByteArray &md5);

    static QVariant readValueAndReport(const QString &key,
            const QVariant &defaultValue, QVariant::Type type = QVariant::String);

    static QString informatorGuidDescription(InformatorError_t err);
    static QString soundInfoTypeListToString(const QList<SoundInfoType_t> &list);
    static InformatorSettings_t *defaultSettings() {
        return &DEFAULT_INF_SETTINGS;
    }
private:
    static const time_struct_informator_t defaultUtc;
    static InformatorSettings_t DEFAULT_INF_SETTINGS;
};




#endif // CINFORMATORMISC_H
