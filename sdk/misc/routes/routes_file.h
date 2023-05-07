#ifndef ROUTES_FILE_H
#define ROUTES_FILE_H

#include <misc/informator/informator_defines.h>
#include <misc/routes/routes.h>
#include <misc/routes/routes_file.h>
#include <QObject>
#include <QVector>




// Класс для сохранения списка маршрутов в файл
class RoutesFile: public QObject
{
    Q_OBJECT
public:
    RoutesFile(QObject *parent = nullptr);
    bool loadFromFile(const QString &fileName);
    bool saveToFile(const QString &filename) const;
    const QVector<RouteFullInfo_t> &routes() const;
    void setRoutes(const QVector<RouteFullInfo_t> &routes);
    bool isFileUnsaved(const QString &fileName) const;
public slots:

private:
    QVector<RouteFullInfo_t> mRoutes;
    QVector<SoundTemplate_t> mTemplates; // Перечень звуковых шаблонов

    static bool loadFromFilePrivate(const QString &fileName,
            QVector<RouteFullInfo_t> &routes,
            QVector<SoundTemplate_t> &templates);

    static InformatorRouteType_t stringToRouteType(const QString &str);

    static QString routeTypeToString(InformatorRouteType_t type);

    static QJsonArray makeOtherLanguagesJsonArray(const QVector<OtherLanguagesData_t> &data);
    static QVector<OtherLanguagesData_t> jsonToOtherLanguagesData(const QJsonArray &jsonArr);

    static QJsonObject makeLedPanelSettingsJsonObject(const LedPanelsSettings &ledPanelSettings);
    static LedPanelsSettings makeLedPanelSettingsJsonObject(const QJsonObject &jsonObj);


    static ELedPanelScrolingType stringToELedPanelScrolingType(const QString &str);
    static QString eLedPanelScrolingTypeToString(ELedPanelScrolingType location);
    static ELedPanelImageLocation stringToELedPanelImageLocation(const QString &str);

    static QString eLedPanelImageLocationToString(ELedPanelImageLocation location);


    static void getBoolJsonValue(const QJsonObject &jsonObj, const QString &valueName,
            bool &value);

};
#endif // ROUTES_FILE_H
