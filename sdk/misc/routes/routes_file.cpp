#include "misc/routes/routes_file.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

RoutesFile::RoutesFile(QObject *parent):
        QObject(parent)
{

}

bool RoutesFile::loadFromFile(const QString &fileName)
{
    return loadFromFilePrivate(fileName, mRoutes, mTemplates);
}


bool RoutesFile::saveToFile(const QString &filename) const
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)) return false;
    QJsonArray routes;
    for(int i = 0; i < mRoutes.size(); i++) // Перебираем автобусы
    {
        const RouteFullInfo_t &route = mRoutes[i];

        // Заполняем route.title
        QJsonObject title;
        title.insert("routeNumber", QJsonValue(route.title.routeNumber));
        title.insert("routeName", QJsonValue(route.title.routeName));
        title.insert("routeFullName", QJsonValue(route.title.routeFullName));
        title.insert("routeType", QJsonValue(routeTypeToString(route.title.routeType)));

        //
        title.insert("defaultLanguage", QJsonValue(route.defaultLanguage));
        title.insert("templatesUid", QJsonValue(QString::number(route.templatesUid, 16)));

        QJsonArray otherLanguages = makeOtherLanguagesJsonArray(route.otherLanguages);

//        QJsonObject ledPanelSettings = makeLedPanelSettingsJsonObject(route.ledPanelSettings);

        QJsonArray subroutes;
        for(int j=0; j<route.title.subroutes.size(); j++) {
            const SubrouteInfo_t &subroute = route.title.subroutes[j];
            QJsonObject subrouteObj;
            subrouteObj.insert("subrouteName", subroute.subrouteName);
            subrouteObj.insert("subrouteSoundPath", QJsonValue(subroute.subrouteSoundPath));
            QJsonArray stationsArr;
            for(int k=0; k<subroute.stations.size(); k++) {
                const StationInfo_t &station = subroute.stations[k];
                QJsonObject stationObj;
                stationObj.insert("stationName", QJsonValue(station.stationName));
                stationObj.insert("lat", QJsonValue(station.lat));
                stationObj.insert("lon", QJsonValue(station.lon));
                stationObj.insert("stationType", QJsonValue(
                        static_cast<int>(station.stationType)));

                QJsonArray controlPoints;
                for(int m=0; m<station.controlPoints.size(); m++) {
                    QJsonObject gateObj;
                    gateObj.insert("pointLat", QJsonValue(
                            station.controlPoints[m].lat));
                    gateObj.insert("pointLon", QJsonValue(
                            station.controlPoints[m].lon));
                    controlPoints.append(QJsonValue(gateObj));
                }
                stationObj.insert("courseGates", QJsonValue(controlPoints));
                stationObj.insert("radius", QJsonValue(station.radius));
                stationObj.insert("audioFileName", QJsonValue(station.audioFileName));
                        stationObj.insert("additionalInfo", QJsonValue(station.additionalInfo));
                stationsArr.append(QJsonValue(stationObj));
            }
            subrouteObj.insert("stations", QJsonValue(stationsArr));
            subroutes.append(subrouteObj);
        }
        QJsonObject routeObj;
        routeObj.insert("title", QJsonValue(title));
        routeObj.insert("otherLanguages", QJsonValue(otherLanguages));
//        routeObj.insert("ledPanelSettings", QJsonValue(ledPanelSettings));
        routeObj.insert("subroutes", QJsonValue(subroutes));
        routes.append(QJsonValue(routeObj));
    }
    QJsonArray templates = SoundTemplates::
            soundTemplatesToJsonArray(mTemplates);
    QJsonObject json;
    json.insert("fileFormat", QJsonValue("A0F1201734235702"));
    json.insert("routes", QJsonValue(routes));
    json.insert("templates", QJsonValue(templates));
    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.flush();
    file.close();
    return true;
}


const QVector<RouteFullInfo_t>& RoutesFile::routes() const
{
    return mRoutes;
}

void RoutesFile::setRoutes(const QVector<RouteFullInfo_t> &routes)
{
    mRoutes = routes;
}


bool RoutesFile::isFileUnsaved(const QString &fileName) const
{
    QVector<RouteFullInfo_t> routes;
    QVector<SoundTemplate_t> templates;
    if(loadFromFilePrivate(fileName, routes, templates)) {
        if(routes == mRoutes && templates == mTemplates) {
            return false;
        }
        else return true;
    }
    else return true;
}


bool RoutesFile::loadFromFilePrivate(const QString &fileName,
        QVector<RouteFullInfo_t> &routes,
        QVector<SoundTemplate_t> &templates)
{
    //bool err = false;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) return false;
    QByteArray json = file.readAll();
    //file.flush();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    // Проверяем формат файла списка маршрутов
    QJsonObject::iterator val = obj.find("fileFormat");
    if(val == obj.end()) return false; // Неизвестный формат файла
    if(!(*val).isString()) return false; // Не тот тип поля "fileFormat"
    QString fileVersion = (*val).toString();
    if(fileVersion != "A0F1201734235701" &&
            fileVersion != "A0F1201734235702") return false; // Неизвестный формат файла

    val = obj.find("routes");
    if(val == obj.end()) {
        return false;
    }
    if(!(*val).isArray()) return false;
    QJsonArray jsonRoutes = (*val).toArray();
    routes.resize(jsonRoutes.size());
    for(int i = 0; i < jsonRoutes.size(); i++) { // Перебираем автобусы
        RouteFullInfo_t &route = routes[i];
        QJsonValue routeVal = jsonRoutes[i];
        if(!routeVal.isObject()) {
            continue;
        }
        QJsonObject routeFullInfo = routeVal.toObject();
        // Парсим title
        QJsonObject::iterator title = routeFullInfo.find("title");
        if(title == routeFullInfo.end()) {
            return false;
        }
        if(!(*title).isObject()) return false;
        QJsonObject tl = (*title).toObject();

        // routeNumber
        QJsonObject::iterator route_number = tl.find("routeNumber");
        if(route_number != tl.end()) {
            QJsonValue v = *route_number;
            if(v.isString())
                route.title.routeNumber = v.toString("строка не распарсена");
            else
                route.title.routeNumber = "";
        }

        // routeName
        QJsonObject::iterator route_name = tl.find("routeName");
        if(route_name != tl.end()) {
            QJsonValue v = *route_name;
            if(v.isString())
                route.title.routeName = v.toString("строка не распарсена");
            else
                route.title.routeName = "";
        }
        else
        {
            route.title.routeName = "";
        }

        // routeFullName
        QJsonObject::iterator route_full_name = tl.find("routeFullName");
        if(route_full_name != tl.end())
        {
            QJsonValue v = *route_full_name;
            if(v.isString())
                route.title.routeFullName = v.toString("строка не распарсена");
            else
                route.title.routeFullName = "";
        }
        else
        {
            route.title.routeFullName = "";
        }

        // routeType
        QJsonObject::iterator route_type = tl.find("routeType");
        if(route_type != tl.end())
        {
            QJsonValue v = *route_type;
            if(v.isDouble())
                route.title.routeType = static_cast<InformatorRouteType_t>(
                        v.toInt(static_cast<int>(InformatorRouteType_t::otherTransport)));
            else if(v.isString())
            {
                route.title.routeType = stringToRouteType(v.toString());
            }
            else
            {
                route.title.routeType = InformatorRouteType_t::otherTransport;
            }
        }

        // defaultLanguage
        QJsonObject::iterator defLang = routeFullInfo.find("defaultLanguage");
        if(defLang != routeFullInfo.end())
        {
            QJsonValue v = *defLang;
            if(v.isString())
                route.defaultLanguage = v.toString("строка не распарсена");
            else
                route.defaultLanguage = "";
        }
        else
        {
            route.defaultLanguage = "";
        }

        // templatesUid
        QJsonObject::iterator templUid = routeFullInfo.find("templatesUid");
        if(templUid != routeFullInfo.end())
        {
            QJsonValue v = *templUid;
            if(v.isString())
            {
                bool ok;
                route.templatesUid = v.toString("строка не распарсена").toInt(&ok, 16);
                if(!ok)
                {
                    route.templatesUid = 0;
                }
            }
            else
            {
                route.templatesUid = 0;
            }
        }
        else
        {
            route.templatesUid = 0;
        }

        //  speakingTowntemplatesUid
        QJsonObject::iterator speakTownUid = routeFullInfo.find("speakingTowntemplatesUid");
        if(speakTownUid != routeFullInfo.end())
        {
            QJsonValue v = *speakTownUid;
            if(v.isString())
            {
                bool ok;
                route.speakingTowntemplatesUid = v.toString("строка не распарсена").toInt(&ok, 16);
                if(!ok)
                {
                    route.speakingTowntemplatesUid = 0;
                }
            }
            else
            {
                route.speakingTowntemplatesUid = 0;
            }
        }
        else
        {
            route.speakingTowntemplatesUid = 0;
        }

        // otherLanguages
        val = routeFullInfo.find("otherLanguages");
        if(val != obj.end())
        {
            if((*val).isArray())
            {
                QJsonArray otherLangArr = (*val).toArray();
                route.otherLanguages = jsonToOtherLanguagesData(otherLangArr);
            }
        }


  /*      // ledPanelSettings
        val = routeFullInfo.find("ledPanelSettings");
        if(val != obj.end())
        {
            if((*val).isObject())
            {
                QJsonObject ledPanelSettings = (*val).toObject();
                route.ledPanelSettings = makeLedPanelSettingsJsonObject(
                        ledPanelSettings);
            }
        }
*/
        // Парсим подмаршруты
        QJsonObject::iterator subroutes_it = routeFullInfo.find("subroutes");
        if(subroutes_it == routeFullInfo.end())
        {
            return false;
        }
        if(!(*subroutes_it).isArray())
        {
            return false;
        }
        QJsonArray subroutes = (*subroutes_it).toArray();
        route.title.subroutes.resize(subroutes.size());
        for(int j=0; j<subroutes.size(); j++)
        {
            QJsonValue subrouteVal = subroutes[j];
            if(!subrouteVal.isObject()) return false;
            SubrouteInfo_t &sbrt = route.title.subroutes[j];
            QJsonObject subrouteObj = subrouteVal.toObject();


            val = subrouteObj.find("subrouteName");
            if(val != subrouteObj.end())
            {
                sbrt.subrouteName = (*val).toString();
            }
            else sbrt.subrouteName = "строка не распарсена";


            val = subrouteObj.find("subrouteSoundPath");
            if(val != subrouteObj.end())
            {
                sbrt.subrouteSoundPath = (*val).toString();
            }
            else sbrt.subrouteSoundPath = "строка не распарсена";


            val = subrouteObj.find("stations");
            if(val == subrouteObj.end()) return false;
            if(!(*val).isArray()) return false;
            QJsonArray stations = (*val).toArray();
            sbrt.stations.resize(stations.size());
            for(int k = 0; k<stations.size(); k++)
            {
                QJsonValue stVal = stations[k];
                if(!stVal.isObject()) continue;
                QJsonObject stationObj = stVal.toObject();
                //val = stationObj.find("Station_num");
                StationInfo_t &st = sbrt.stations[k];

                val = stationObj.find("stationName");
                if(val != stationObj.end())
                {
                    if((*val).isString())
                        st.stationName = (*val).toString();
                    else
                        st.stationName = "";
                }
                else st.stationName = "";

                val = stationObj.find("lat");
                if(val != stationObj.end())
                {
                    if((*val).isDouble())
                        st.lat = (*val).toDouble();
                    else
                        st.lat = -1.0;
                }
                else st.lat = -1.0;

                val = stationObj.find("lon");
                if(val != stationObj.end())
                {
                    if((*val).isDouble())
                        st.lon = (*val).toDouble();
                    else
                        st.lon = -1.0;
                }
                else
                    st.lon = -1.0;

                val = stationObj.find("courseGates");
                if(val != stationObj.end())
                {
                    if((*val).isArray())
                    {
                        QJsonArray arr = (*val).toArray();
                        st.controlPoints.resize(arr.size());
                        for(int m = 0; m< arr.size(); m++)
                        {
                            if(arr[m].isObject())
                            {
                                QJsonObject gateObj = arr[m].toObject();
                                val = gateObj.find("pointLat");
                                if(val!= gateObj.end()) {
                                    if((*val).isDouble()) {
                                        QJsonValue lat = *val;
                                        st.controlPoints[m].lat =
                                                lat.toDouble(-1);
                                    }
                                    else st.controlPoints[m].lat = -1.0;
                                }
                                else st.controlPoints[m].lat = -1.0;
                                val = gateObj.find("pointLon");
                                if(val!= gateObj.end())
                                {
                                    if((*val).isDouble()) {
                                        QJsonValue lon = *val;
                                        st.controlPoints[m].lon =
                                                lon.toDouble(-1);
                                    }
                                    else st.controlPoints[m].lon = -1.0;
                                }
                                else st.controlPoints[m].lon = -1.0;
                            }
                            else st.controlPoints[m] = {-1.0, -1.0};
                        }
                    }
                    else st.controlPoints = QVector<StationControlPoint_t>();
                }
                else
                    st.controlPoints = QVector<StationControlPoint_t>();

                val = stationObj.find("radius");
                if(val != stationObj.end())
                {
                    if((*val).isDouble())
                        st.radius = (*val).toDouble();
                    else
                        st.radius = -1.0;
                }
                else st.radius = -1.0;

                val = stationObj.find("audioFileName");
                if(val != stationObj.end())
                {
                    if((*val).isString())
                        st.audioFileName = (*val).toString();
                    else
                        st.audioFileName = "";
                }
                else st.audioFileName = "";

                val = stationObj.find("additionalInfo");
                if(val != stationObj.end())
                {
                    if((*val).isString())
                        st.additionalInfo = (*val).toString();
                    else
                        st.additionalInfo = "";
                }
                else st.additionalInfo = "";

                //obj.insert("Station_lon", QJsonValue(subRoute.stations[i].mainInfo.lon));

                val = stationObj.find("stationType");
                if(val != stationObj.end())
                {
                    if((*val).isDouble())
                        st.stationType = static_cast<StationType_t>((*val).toInt());
                    else
                        st.stationType = StationType_t::unknownType;
                }
                else st.stationType = StationType_t::unknownType;
            }
        }
    }

    val = obj.find("templates");
    if(val != obj.end())
    {
        if((*val).isArray())
        {
            QJsonArray jsonTemplates = (*val).toArray();
            bool err = true;
            templates = SoundTemplates::jsonArrayToSoundTemplates(jsonTemplates, err);
            if(err)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

QJsonArray RoutesFile::makeOtherLanguagesJsonArray(const QVector<OtherLanguagesData_t> &data)
{
    QJsonArray arr;
    for(int i=0; i<data.size(); i++)
    {
        const OtherLanguagesData_t &lang = data[i];
        QJsonObject languageData;
        languageData.insert("languageName", QJsonValue(lang.languageName));
        languageData.insert("routeNumber", QJsonValue(lang.routeNumber));
        languageData.insert("routeName", QJsonValue(lang.routeName));
        languageData.insert("routeFullName", QJsonValue(lang.routeFullName));
        languageData.insert("routeSoundPath", QJsonValue(lang.routeSoundPath));
        //title.insert("routeEnding1SoundPath", QJsonValue(lang.routeEnding1SoundPath));
        //title.insert("routeEnding2SoundPath", QJsonValue(lang.routeEnding1SoundPath));
        QJsonArray subroutesJson;
        for(int j=0; j<lang.subroutes.size(); j++)
        {
            const OtherLanguagesSubroute_t &subroute = lang.subroutes[j];
            QJsonObject subrouteData;
            subrouteData.insert("subrouteName", QJsonValue(subroute.subrouteName));

            QJsonArray stationsJson;
            for(int k=0; k<subroute.stations.size(); k++)
            {
                const OtherLanguageStationInfo_t &station = subroute.stations[k];
                QJsonObject stationData;
                stationData.insert("stationName", QJsonValue(station.stationName));
                stationData.insert("audioFileName", QJsonValue(station.audioFileName));
                stationData.insert("additionalInfo", QJsonValue(station.additionalInfo));
                stationsJson.append(QJsonValue( stationData));
            }

            QJsonArray voicePointsJson;
            for(int k=0; k<subroute.voicePoints.size(); k++)
            {
                const OtherLanguageStationInfo_t &vp = subroute.voicePoints[k];
                QJsonObject voicePointData;
                voicePointData.insert("stationName", QJsonValue(vp.stationName));
                voicePointData.insert("audioFileName", QJsonValue(vp.audioFileName));
                voicePointData.insert("additionalInfo", QJsonValue(vp.additionalInfo));
                voicePointsJson.append(QJsonValue( voicePointData));
            }

            subrouteData.insert("stations", QJsonValue(stationsJson));
            subrouteData.insert("voicePoints", QJsonValue(voicePointsJson));

            subroutesJson.append(QJsonValue(subrouteData));
        }

        languageData.insert("subroutes", QJsonValue(subroutesJson));
        arr.append(QJsonValue(languageData));
    }
    return arr;
}


QVector<OtherLanguagesData_t> RoutesFile::jsonToOtherLanguagesData(
        const QJsonArray &jsonArr)
{
    QVector<OtherLanguagesData_t> langs;
    langs.resize(jsonArr.size());
    for(int i=0; i<jsonArr.size(); i++)
    {
        OtherLanguagesData_t &lang = langs[i];
        QJsonValue jsVal = jsonArr[i];
        if(!jsVal.isObject()) return langs;
        QJsonObject languageObj = jsVal.toObject();

        QJsonObject::iterator it;
        // languageName
        it = languageObj.find("languageName");
        if(it != languageObj.end())
        {
            QJsonValue v = *it;
            if(v.isString())
                lang.languageName = v.toString("строка не распарсена");
            else
                lang.languageName = "";
        }

        // routeNumber
        it = languageObj.find("routeNumber");
        if(it != languageObj.end())
        {
            QJsonValue v = *it;
            if(v.isString())
                lang.routeNumber = v.toString("строка не распарсена");
            else
                lang.routeNumber = "";
        }

        // routeName
        it = languageObj.find("routeName");
        if(it != languageObj.end())
        {
            QJsonValue v = *it;
            if(v.isString())
                lang.routeName = v.toString("строка не распарсена");
            else
                lang.routeName = "";
        }

        // routeFullName
        it = languageObj.find("routeFullName");
        if(it != languageObj.end())
        {
            QJsonValue v = *it;
            if(v.isString())
                lang.routeFullName = v.toString("строка не распарсена");
            else
                lang.routeFullName = "";
        }

        // routeSoundPath
        it = languageObj.find("routeSoundPath");
        if(it != languageObj.end())
        {
            QJsonValue v = *it;
            if(v.isString())
                lang.routeSoundPath = v.toString("строка не распарсена");
            else
                lang.routeSoundPath = "";
        }

        // Парсим подмаршруты
        it = languageObj.find("subroutes");
        if(it == languageObj.end())
        {
            return langs;
        }
        if(!(*it).isArray())
        {
            return langs;
        }
        QJsonArray subroutes = (*it).toArray();
        lang.subroutes.resize(subroutes.size());
        for(int j=0; j<subroutes.size(); j++)
        {
            QJsonValue subrouteVal = subroutes[j];
            if(!subrouteVal.isObject()) continue;
            OtherLanguagesSubroute_t &sbrt = lang.subroutes[j];
            QJsonObject subrouteObj = subrouteVal.toObject();
            it = subrouteObj.find("subrouteName");

            if(it != subrouteObj.end())
            {
                sbrt.subrouteName = (*it).toString();
            }
            else sbrt.subrouteName = "строка не распарсена";

            // Остановки
            it = subrouteObj.find("stations");
            if(it == subrouteObj.end())
            {
                return langs;
            }
            if(!(*it).isArray())
            {
                return langs;
            }
            QJsonArray stations = (*it).toArray();
            sbrt.stations.resize(stations.size());
            for(int k = 0; k<stations.size(); k++)
            {
                QJsonValue stVal = stations[k];
                if(!stVal.isObject()) continue;
                QJsonObject stationObj = stVal.toObject();
                OtherLanguageStationInfo_t &st = sbrt.stations[k];

                // stationName
                it = stationObj.find("stationName");
                if(it != stationObj.end())
                {
                    if((*it).isString())
                        st.stationName = (*it).toString();
                    else
                        st.stationName = "";
                }
                else st.stationName = "";

                // audioFileName
                it = stationObj.find("audioFileName");
                if(it != stationObj.end())
                {
                    if((*it).isString())
                        st.audioFileName = (*it).toString();
                    else
                        st.audioFileName = "";
                }
                else st.audioFileName = "";

                // additionalInfo
                it = stationObj.find("additionalInfo");
                if(it != stationObj.end())
                {
                    if((*it).isString())
                        st.additionalInfo = (*it).toString();
                    else
                        st.additionalInfo = "";
                }
                else st.additionalInfo = "";
            }

            // voicePoints - контрольные точки
            it = subrouteObj.find("voicePoints");
            if(it == subrouteObj.end())
            {
                return langs;
            }
            if(!(*it).isArray())
            {
                return langs;
            }
            QJsonArray voicePoints = (*it).toArray();
            sbrt.voicePoints.resize(voicePoints.size());
            for(int k = 0; k<voicePoints.size(); k++)
            {
                QJsonValue voicePointsVal = voicePoints[k];
                if(!voicePointsVal.isObject()) continue;
                QJsonObject voicePointsValObj = voicePointsVal.toObject();

                OtherLanguageStationInfo_t &vp = sbrt.voicePoints[k];

                // stationName
                it = voicePointsValObj.find("stationName");
                if(it != voicePointsValObj.end())
                {
                    if((*it).isString())
                        vp.stationName = (*it).toString();
                    else
                        vp.stationName = "";
                }
                else vp.stationName = "";

                // audioFileName
                it = voicePointsValObj.find("audioFileName");
                if(it != voicePointsValObj.end())
                {
                    if((*it).isString())
                        vp.audioFileName = (*it).toString();
                    else
                        vp.audioFileName = "";
                }
                else vp.audioFileName = "";

                // additionalInfo
                it = voicePointsValObj.find("additionalInfo");
                if(it != voicePointsValObj.end())
                {
                    if((*it).isString())
                        vp.additionalInfo = (*it).toString();
                    else
                        vp.additionalInfo = "";
                }
                else vp.additionalInfo = "";
            }
        }

    }
    return langs;
}


/*QJsonObject RoutesFile::makeLedPanelSettingsJsonObject(const LedPanelsSettings &ledPanelSettings)
{
    QJsonObject res;
    res.insert("enableApplySettings", QJsonValue(ledPanelSettings.enableApplySettings));
    res.insert("scrolingType", QJsonValue(eLedPanelScrolingTypeToString(ledPanelSettings.scrolingType)));

    res.insert("wideListStationsOutEnabled", QJsonValue(ledPanelSettings.wideListStationsOutEnabled));
    res.insert("allStationsToOneString", QJsonValue(ledPanelSettings.allStationsToOneString));
    res.insert("sidePanelLikeFrontOne", QJsonValue(ledPanelSettings.sidePanelLikeFrontOne));
    res.insert("frontPanelLikeSideOne", QJsonValue(ledPanelSettings.frontPanelLikeSideOne));
    res.insert("startAndEndStationsToFrontPanel", QJsonValue(ledPanelSettings.startAndEndStationsToFrontPanel));
    res.insert("endStationsOnlyToFrontPanelAsRunningString", QJsonValue(ledPanelSettings.endStationsOnlyToFrontPanelAsRunningString));
    res.insert("endStationsOnlyToFrontPanelAsStaticText", QJsonValue(ledPanelSettings.endStationsOnlyToFrontPanelAsStaticText));
    res.insert("fullNamesAndAdverToFrondPanel", QJsonValue(ledPanelSettings.fullNamesAndAdverToFrondPanel));
    res.insert("fullNamesAndAdverToSidePanel", QJsonValue(ledPanelSettings.fullNamesAndAdverToSidePanel));
    res.insert("timeRenderDisable", QJsonValue(ledPanelSettings.timeRenderDisable));
    res.insert("insideTemperRenderDisable", QJsonValue(ledPanelSettings.insideTemperRenderDisable));
    res.insert("disableRouteNumberToFrontPanel", QJsonValue(ledPanelSettings.disableRouteNumberToFrontPanel));
    res.insert("disableeRouteNumberToSidePanel", QJsonValue(ledPanelSettings.disableeRouteNumberToSidePanel));

    res.insert("imagePath", QJsonValue(ledPanelSettings.imagePath));

    res.insert("imageLocationFrontPanel", QJsonValue(eLedPanelImageLocationToString(ledPanelSettings.imageLocationFrontPanel)));
    res.insert("imageLocationSidePanel", QJsonValue(eLedPanelImageLocationToString(ledPanelSettings.imageLocationSidePanel)));
    res.insert("imageLocationBackPanel", QJsonValue(eLedPanelImageLocationToString(ledPanelSettings.imageLocationBackPanel)));
    return res;
}

LedPanelsSettings RoutesFile::makeLedPanelSettingsJsonObject(
        const QJsonObject &jsonObj)
{
    LedPanelsSettings settings;

    // enableApplySettings
    getBoolJsonValue(jsonObj, "enableApplySettings", settings.enableApplySettings);

    // enableApplySettings
    QJsonObject::const_iterator it = jsonObj.find("scrolingType");
    if(it != jsonObj.end())
    {
        QJsonValue v = *it;
        if(v.isString())
        {
            settings.scrolingType = stringToELedPanelScrolingType(v.toString());
        }
        else
        {
            settings.scrolingType = ELedPanelScrolingType::horisontalAsList;
        }
    }
    else
    {
        settings.scrolingType = ELedPanelScrolingType::horisontalAsList;
    }

    getBoolJsonValue(jsonObj, "wideListStationsOutEnabled", settings.wideListStationsOutEnabled);
    getBoolJsonValue(jsonObj, "allStationsToOneString", settings.allStationsToOneString);
    getBoolJsonValue(jsonObj, "sidePanelLikeFrontOne", settings.sidePanelLikeFrontOne);
    getBoolJsonValue(jsonObj, "frontPanelLikeSideOne", settings.frontPanelLikeSideOne);
    getBoolJsonValue(jsonObj, "startAndEndStationsToFrontPanel", settings.startAndEndStationsToFrontPanel);
    getBoolJsonValue(jsonObj, "endStationsOnlyToFrontPanelAsRunningString", settings.endStationsOnlyToFrontPanelAsRunningString);
    getBoolJsonValue(jsonObj, "endStationsOnlyToFrontPanelAsStaticText", settings.endStationsOnlyToFrontPanelAsStaticText);
    getBoolJsonValue(jsonObj, "fullNamesAndAdverToFrondPanel", settings.fullNamesAndAdverToFrondPanel);
    getBoolJsonValue(jsonObj, "fullNamesAndAdverToSidePanel", settings.fullNamesAndAdverToSidePanel);
    getBoolJsonValue(jsonObj, "timeRenderDisable", settings.timeRenderDisable);
    getBoolJsonValue(jsonObj, "insideTemperRenderDisable", settings.insideTemperRenderDisable);
    getBoolJsonValue(jsonObj, "disableRouteNumberToFrontPanel", settings.disableRouteNumberToFrontPanel);
    getBoolJsonValue(jsonObj, "disableeRouteNumberToSidePanel", settings.disableeRouteNumberToSidePanel);

    // imagePath
    it = jsonObj.find("imagePath");
    if(it != jsonObj.end())
    {
        QJsonValue v = *it;
        if(v.isString())
        {
            settings.imagePath = v.toString();
        }
        else
        {
            settings.imagePath = "";
        }
    }
    else
    {
        settings.imagePath = "";
    }

    // imageLocationFrontPanel
    it = jsonObj.find("imageLocationFrontPanel");
    if(it != jsonObj.end())
    {
        QJsonValue v = *it;
        if(v.isString())
        {
            settings.imageLocationFrontPanel = stringToELedPanelImageLocation(v.toString());
        }
        else
        {
            settings.imageLocationFrontPanel = ELedPanelImageLocation::dontShow;
        }
    }
    else
    {
        settings.imageLocationFrontPanel = ELedPanelImageLocation::dontShow;
    }

    // imageLocationSidePanel
    it = jsonObj.find("imageLocationSidePanel");
    if(it != jsonObj.end())
    {
        QJsonValue v = *it;
        if(v.isString())
        {
            settings.imageLocationSidePanel = stringToELedPanelImageLocation(v.toString());
        }
        else
        {
            settings.imageLocationSidePanel = ELedPanelImageLocation::dontShow;
        }
    }
    else
    {
        settings.imageLocationSidePanel = ELedPanelImageLocation::dontShow;
    }

    // imageLocationBackPanel
    it = jsonObj.find("imageLocationBackPanel");
    if(it != jsonObj.end())
    {
        QJsonValue v = *it;
        if(v.isString())
        {
            settings.imageLocationBackPanel = stringToELedPanelImageLocation(v.toString());
        }
        else
        {
            settings.imageLocationBackPanel = ELedPanelImageLocation::dontShow;
        }
    }
    else
    {
        settings.imageLocationBackPanel = ELedPanelImageLocation::dontShow;
    }
    return settings;
}
*/

void RoutesFile::getBoolJsonValue(const QJsonObject &jsonObj, const QString &valueName, bool &value)
{
    QJsonObject::ConstIterator it = jsonObj.find(valueName);
    if(it != jsonObj.end())
    {
        QJsonValue v = *it;
        if(v.isBool())
            value = v.toBool(false);
        else
            value = false;
    }
    else value = false;
}





InformatorRouteType_t RoutesFile::stringToRouteType(const QString &str)
{
    if(str == "bus")                            // автобус
        return InformatorRouteType_t::bus;
    else if(str == "aeroexpress")                    // аэроекспресс
        return InformatorRouteType_t::aeroexpress;
    else if(str == "waterTransport")                 // водный транспорт
        return InformatorRouteType_t::waterTransport;
    else if(str == "cableСar")                       // Канатная дорога
        return InformatorRouteType_t::cableCar;
    else if(str == "lightMetro")                     // Легкое метро
        return InformatorRouteType_t::lightMetro;
    else if(str == "fixedRouteTaxi")                 // Маршрутное такси
        return InformatorRouteType_t::fixedRouteTaxi;
    else if(str == "metro")                          // Метро
        return InformatorRouteType_t::metro;
    else if(str == "metrotram")                      // Метротрам
        return InformatorRouteType_t::metrotram;
    else if(str == "monoreil")				 // Монорельс
        return InformatorRouteType_t::monoreil;
    else if(str == "speedTram")				 // Скоростной трамвай
        return InformatorRouteType_t::speedTram;
    else if(str == "train")				 // Поезд
        return InformatorRouteType_t::train;
    else if(str == "tram")				// трамвай
        return InformatorRouteType_t::tram;
    else if(str == "trolleybus")				// тролейбус
        return InformatorRouteType_t::trolleybus;
    else if(str == "funicular")				// Фуникулер
        return InformatorRouteType_t::funicular;
    else if(str == "electroTrain")				// Электропоезд
        return InformatorRouteType_t::electroTrain;
    else return InformatorRouteType_t::otherTransport;
}


QString RoutesFile::routeTypeToString(InformatorRouteType_t type)
{
    switch(type)
    {
    case InformatorRouteType_t::bus: return "bus";
    case InformatorRouteType_t::aeroexpress: return "aeroexpress";
    case InformatorRouteType_t::waterTransport: return "waterTransport";
    case InformatorRouteType_t::cableCar: return "cableСar";
    case InformatorRouteType_t::lightMetro: return "lightMetro";
    case InformatorRouteType_t::fixedRouteTaxi: return "fixedRouteTaxi";
    case InformatorRouteType_t::metro: return "metro";
    case InformatorRouteType_t::metrotram: return "metrotram";
    case InformatorRouteType_t::monoreil: return "monoreil";
    case InformatorRouteType_t::speedTram: return "speedTram";
    case InformatorRouteType_t::train: return "train";
    case InformatorRouteType_t::tram: return "tram";
    case InformatorRouteType_t::trolleybus: return "trolleybus";
    case InformatorRouteType_t::funicular: return "funicular";
    case InformatorRouteType_t::electroTrain: return "electroTrain";
    default: return "otherTransport";
    }
}




ELedPanelScrolingType RoutesFile::stringToELedPanelScrolingType(const QString &str)
{
    if(str == "vertical") return ELedPanelScrolingType::vertical;
    else if(str == "horisontalOneStation") return ELedPanelScrolingType::horisontalOneStation;
    else return ELedPanelScrolingType::horisontalAsList;
}

QString RoutesFile::eLedPanelScrolingTypeToString(ELedPanelScrolingType location)
{
    switch(location)
    {
    case ELedPanelScrolingType::vertical: return "vertical";
    case ELedPanelScrolingType::horisontalOneStation: return "horisontalOneStation";
    default: return "horisontalAsList";
    }
}



ELedPanelImageLocation RoutesFile::stringToELedPanelImageLocation(const QString &str)
{
    if(str == "replaceRouteNumber") return ELedPanelImageLocation::replaceRouteNumber;
    else if(str == "beforeRouteNumber") return ELedPanelImageLocation::beforeRouteNumber;
    else if(str == "afterNumber") return ELedPanelImageLocation::afterNumber;
    else
    {
        return ELedPanelImageLocation::dontShow;
    }
}

QString RoutesFile::eLedPanelImageLocationToString(ELedPanelImageLocation location)
{
    switch(location)
    {
    case ELedPanelImageLocation::replaceRouteNumber: return "replaceRouteNumber";
    case ELedPanelImageLocation::beforeRouteNumber: return "beforeRouteNumber";
    case ELedPanelImageLocation::afterNumber: return "afterNumber";
    default: return "dontShow";
    }
}





