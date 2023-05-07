#ifndef ROUTESPARSER_H
#define ROUTESPARSER_H

// STL
#include <functional>

// Qt
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QUrl>
#include <QDebug>

//SDK
#include "Mqtt/Topics/Informator/InformatorDefines.h"

#include "RouteNetworkStructures.h"


class RoutesParser
{
public:
    RoutesParser(const QByteArray& raw_json_data)
    {
        m_json_doc = QJsonDocument::fromJson(QString(raw_json_data).toUtf8());
    }

    ~RoutesParser() {}
    bool isNull() const
    {
        return m_json_doc.isNull();
    }

    QList<qint64> routesID() const
    {
        QList<qint64> result;

        QJsonObject obj = m_json_doc.object();

        if (!obj.isEmpty())
        {
            QJsonArray routes = obj["routes"].toArray();

            for (const auto route : routes)
                result.append(route.toObject().value("idRoute").toInt());
        }

        return result;
    }

    QString idDepot() const
    {
        QVariant result;
        if (!isNull())
        {
            QJsonObject json_obj = m_json_doc.object();
            if (!json_obj.isEmpty())
            {
                result = json_obj["idDepot"].toVariant();
            }
        }
        return result.toString();
    }

    QString depotName() const
    {
        QVariant result;

        QJsonObject json_obj = m_json_doc.object();
        if (!json_obj.isEmpty())
        {
            result = json_obj["name"].toVariant();
        }

        return result.toString();
    }

    QString agency() const
    {
        QVariant result;

        QJsonObject json_obj = m_json_doc.object();
        if (!json_obj.isEmpty())
        {
            result = json_obj["idAgency"].toVariant();
        }

        return result.toString();
    }

    QString title(const qint64 route_id) const
    {
        QString result;
        if (!isNull())
        {
            QJsonObject json_obj = m_json_doc.object();

            if (!json_obj.isEmpty())
            {
                QJsonArray routes = json_obj["routes"].toArray();
                QJsonValue route = findValueById(routes, route_id, "idRoute");
                if (!route.isNull())
                {
                    result = route.toObject()["route"].toString();
                }
            }
        }

        return result;
    }

    QList<QPair<qint64, QList<qint64>>> tripsPerRoute()
    {
        QList<QPair<qint64, QList<qint64>>> result;

        if (!isNull())
        {
            QJsonObject json_obj = m_json_doc.object();
            if (!json_obj.isEmpty())
            {
                QJsonArray routes = json_obj["routes"].toArray();
                for (const auto route: routes)
                {
                    QJsonObject route_obj = route.toObject();
                    qint64 idRoute = route_obj["idRoute"].toInt();
                    QJsonArray trips = route_obj["trips"].toArray();
                    QList<qint64> tripsList;
                    foreach (const QJsonValue& trip, trips)
                    {
                        qint64 idTrip = trip.toObject()["idTrip"].toInt();
                        tripsList.push_back(idTrip);
                    }
                    result.push_back({idRoute, tripsList});
                }
            }
        }

        return result;
    }

    LedSettings_t ledPanelSettings(const qint64 route_id)
    {
        LedSettings_t result;
        if (!isNull())
        {
            QJsonObject json_obj = m_json_doc.object();

            if (!json_obj.isEmpty())
            {
                QJsonArray routes = json_obj["routes"].toArray();
                QJsonValue route = findValueById(routes, route_id, "idRoute");
                if (!route.isNull())
                {
                    QJsonObject route_obj = route.toObject()["ledPanelSettings"].toObject();
                    result.frontPanelHAlign = route_obj["frontPanelHAlign"].toVariant().toString();
                    result.sidePanelHAlign = route_obj["sidePanelHAlign"].toVariant().toString();
                    result.salonPanelHAlign = route_obj["salonPanelHAlign"].toVariant().toString();
                    result.routeNumberColorText = route_obj["routeNumberColorText"].toString();
                    result.routeNumberColorBackGround = route_obj["routeNumberColorBackGround"].toString();
                    result.textNumberColorText = route_obj["textNumberColorText"].toString();
                    result.textNumberColorBackGround = route_obj["textNumberColorBackGround"].toString();
                }
            }
        }
        return result;
    }

    Resources_t routeResources(const qint64 route_id)
    {
        Resources_t result;
        if (!isNull())
        {
            QJsonObject json_obj = m_json_doc.object();

            if (!json_obj.isEmpty())
            {
                QJsonArray routes = json_obj["routes"].toArray();
                QJsonValue route = findValueById(routes, route_id, "idRoute");
                if (!route.isNull())
                {
                    QJsonObject text_rec_obj = route.toObject()["resources"].toObject()["text"].toObject();
                    result.text = fillTextResources(text_rec_obj);
                    result.audio.audioData = fillAudioResources(route);
                }
            }
        }
        return result;
    }

    SubrouteInfo_t subroute(const qint64 route_id, const qint64 trip_id, bool& search_trip_id_result) const
    {
        SubrouteInfo_t result;
        search_trip_id_result = false;
        QVector<SubrouteInfo_t> subroutes_list = subroutes(route_id);

        for (auto it = subroutes_list.cbegin(); it != subroutes_list.cend(); ++it)
        {
            SubrouteInfo_t subroute_info = *it;
            if (subroute_info.idTrip == trip_id)
            {
                result = subroute_info;
                search_trip_id_result = true;
                break;
            }
        }
        return result;
    }

    QVector<SubrouteInfo_t> subroutes(const qint64 route_id) const
    {
        QVector<SubrouteInfo_t> result;

        if (!isNull())
        {
            QJsonObject json_obj = m_json_doc.object();

            if (!json_obj.isEmpty())
            {
                QJsonArray routes = json_obj["routes"].toArray();
                QJsonValue route;
                route = findValueById(routes, route_id, "idRoute");

                if (!route.isNull())
                {
                    QJsonArray trips = route.toObject()["trips"].toArray();
                    foreach (const QJsonValue& trip, trips)
                    {
                        SubrouteInfo_t subroute_info;
                        subroute_info.idTrip = trip.toObject()["idTrip"].toInt();
                        subroute_info.shortDescription = trip.toObject()["shortDescription"].toString();
                        subroute_info.roadTrack = fillRoadTrack(trip);
                        subroute_info.tripType = fillTripDirection(trip.toObject()["tripType"].toVariant());
                        subroute_info.subrouteName = trip.toObject()["trip"].toString();
                        subroute_info.subrouteSoundPath = "";
                        QJsonObject text_rec_obj = trip.toObject()["resources"].toObject()["text"].toObject();
                        subroute_info.resources.text = fillTextResources(text_rec_obj);
                        subroute_info.resources.audio.audioData = fillAudioResources(route);
                        subroute_info.resources.externalSpeaker.audioData = fillExternalSpeakerResources(route);
                        QJsonArray stations = trip.toObject()["stations"].toArray();

                        foreach (const QJsonValue& value, stations)
                        {
                            StationInfo_t station_info;
                            QJsonObject obj = value.toObject();
                            station_info.sorting = obj["sorting"].toString();
                            station_info.idStation = obj["idStation"].toVariant().toString();
                            station_info.lon = obj["lon"].toDouble();
                            station_info.lat = obj["lat"].toDouble();
                            station_info.stationName = obj["name"].toString();
                            station_info.controlPoints = {};
                            QVariant station_type = obj["stationType"].toVariant();
                            station_info.stationType = fillStationType(station_type);
                            station_info.resources.audio.stopOut = fillStationAudioResources(value, "stop_out");
                            station_info.resources.audio.stopIn = fillStationAudioResources(value, "stop_in");
                            station_info.resources.audio.stopOver = fillStationAudioResources(value, "stop_over");
                            QJsonObject text_rec_obj = obj["resources"].toObject()["text"].toObject();
                            QJsonObject after_text_rec_obj = text_rec_obj["stop_out"].toObject();
                            QJsonObject before_text_rec_obj = text_rec_obj["stop_in"].toObject();
                            QJsonObject on_text_rec_obj = text_rec_obj["stop_over"].toObject();
                            station_info.resources.text.stopOut = fillTextResources(after_text_rec_obj);
                            station_info.resources.text.stopIn = fillTextResources(before_text_rec_obj);
                            station_info.resources.text.stopOver = fillTextResources(on_text_rec_obj);
                            subroute_info.stations.push_back(station_info);
                        }
                        result.push_back(subroute_info);
                    }
                }
            }
        }

        return result;
    }

    const QList<Audio_t> audiosInfo() const
    {
        QList<Audio_t> result;
        QList<QString> urls;
        std::function<void(const QJsonArray&, QList<Audio_t>&, QList<QString>&)> fillAudioInfo = [](	const QJsonArray& arr,
                QList<Audio_t>& res,
                QList<QString>& urls)
        {
            foreach (const QJsonValue& station, arr)
            {
                Audio_t audio;

                QJsonObject audio_object = station.toObject();

                audio.id = audio_object["id"].toInt();
                audio.url = audio_object["url"].toString();
                audio.name = audio_object["crcSHA256"].toString() + ".mp3";
                audio.pause = audio_object["pause"].toDouble();
                audio.sorting = audio_object["sorting"].toInt();
                audio.fileSize = audio_object["filesize"].toInt();
                audio.crc = audio_object["crcSHA256"].toString();

                if (!audio.url.isEmpty() && !urls.contains(audio.url))
                {
                    res.push_back(audio);
                    urls.push_back(audio.url);
                }
            }
        };

        if (!isNull())
        {
            QJsonObject json_obj = m_json_doc.object();
            if (!json_obj.isEmpty())
            {
                QJsonArray routes = json_obj["routes"].toArray();
                foreach (const QJsonValue& route, routes)
                {
                    QJsonObject route_obj = route.toObject();
                    QJsonArray trips = route_obj["trips"].toArray();
                    foreach (const QJsonValue& trip, trips)
                    {
                        QJsonArray stations = trip.toObject()["stations"].toArray();
                        foreach (const QJsonValue& station, stations)
                        {
                            QJsonObject station_obj = station.toObject();
                            QJsonObject resources_obj = station_obj["resources"].toObject()["audio"].toObject();
                            QJsonArray afterstations_arr = resources_obj["stop_out"].toArray();
                            QJsonArray beforestations_arr = resources_obj["stop_in"].toArray();
                            QJsonArray onstations_arr = resources_obj["stop_over"].toArray();

                            fillAudioInfo(afterstations_arr, result, urls);
                            fillAudioInfo(beforestations_arr, result, urls);
                            fillAudioInfo(onstations_arr, result, urls);
                        }
                    }
                }
            }
        }
        return result;
    }

    QList<QPair<QString,QString>> filesUrls() const
    {
        QList<QPair<QString,QString>> result;
        std::function<void(const QJsonArray&, QList<QPair<QString,QString>>&)> fillUrlsList = [](const QJsonArray& arr, QList<QPair<QString,QString>>& res)
        {
            foreach (const QJsonValue& station, arr)
            {
                QJsonObject audio_object = station.toObject();
                QString url = audio_object["url"].toString();
                QString name = audio_object["crcSHA256"].toString() + ".mp3";
                if (!res.contains({name, url}))
                    res.push_back({name, url});
            }
        };

        if (!isNull())
        {
            QJsonObject json_obj = m_json_doc.object();
            if (!json_obj.isEmpty())
            {
                QJsonArray routes = json_obj["routes"].toArray();
                foreach (const QJsonValue& route, routes)
                {
                    QJsonObject route_obj = route.toObject();
                    QJsonArray trips = route_obj["trips"].toArray();
                    foreach (const QJsonValue& trip, trips)
                    {
                        QJsonArray stations = trip.toObject()["stations"].toArray();
                        foreach (const QJsonValue& station, stations)
                        {
                            QJsonObject station_obj = station.toObject();
                            QJsonObject resources_obj = station_obj["resources"].toObject()["audio"].toObject();
                            QJsonArray afterstations_arr = resources_obj["stop_out"].toArray();
                            QJsonArray beforestations_arr = resources_obj["stop_in"].toArray();
                            QJsonArray onstations_arr = resources_obj["stop_over"].toArray();

                            fillUrlsList(afterstations_arr, result);
                            fillUrlsList(beforestations_arr, result);
                            fillUrlsList(onstations_arr, result);
                        }
                    }
                }
            }
        }

        return result;
    }

private:
    QJsonValue findValueById(const QJsonArray& arr, const qint64 route_id, const QString& field_name) const
    {
        foreach (const QJsonValue& value, arr)
        {
            QJsonObject obj = value.toObject();
            QVariant current_route_id = obj[field_name].toVariant();
            if (current_route_id == route_id)
                return value;
        }
        return QJsonValue();
    }

    QVector<QVector<double>> fillRoadTrack(const QJsonValue& trip) const
    {
        QVector<QVector<double>> result;
        QJsonObject tripObject = trip.toObject();
        QJsonArray roadTrackPoints = tripObject["roadTrack"].toArray();
        foreach (const QJsonValue& pointsValue, roadTrackPoints)
        {
            QVector<double> points;
            auto pointsJSonArr = pointsValue.toArray();
            for (auto it = pointsJSonArr.begin(); it != pointsJSonArr.end(); ++it)
            {
                points.push_back((*it).toDouble());
            }
            result.push_back(points);
        }

        return result;
    }

    StationType_t fillStationType(const QVariant& type) const
    {
        switch (type.toInt())
        {
            case 0:
            return StationType_t::initial;
            case 1:
            return StationType_t::mediate;
            case 2:
            return StationType_t::finite;
            case 3:
            return StationType_t::message;
            case 4:
            return StationType_t::unknownType;
            default:
            return StationType_t::unknownType;
        }
    }

    TripDirection fillTripDirection(const QVariant& type) const
    {
        switch (type.toInt())
        {
            case 0:
            return TripDirection::Straight;
            case 1:
            return  TripDirection::Reverse;
            case 2:
            return  TripDirection::Round;
            default:
            return  TripDirection::UnKnown;
        }
    }

    TextResource_t fillTextResources(const QJsonObject& resources_obj) const
    {
        TextResource_t result;
        QStringList resources_types;
        resources_types << "led_num" << "led_side" << "led_front" << "led_internal" << "lcd_text";

        foreach (const QString& resources_type, resources_types)
        {
            if (resources_obj.contains(resources_type))
            {
                TextRC_t text_rc = fillTextRC(resources_obj[resources_type].toObject());
                if (resources_type == "led_num")
                {
                    result.ledNum = text_rc;
                }
                else if (resources_type == "led_side")
                {
                    result.ledSide = text_rc;
                }
                else if (resources_type == "led_front")
                {
                    result.ledFront = text_rc;
                }
                else if (resources_type == "led_internal")
                {
                    result.ledInternal = text_rc;
                }
                else if (resources_type == "lcd_text")
                {
                    result.lcdText = text_rc;
                }
            }
        }

        return result;
    }

    TextRC_t fillTextRC(const QJsonObject& obj) const
    {
        TextRC_t result;
        QStringList lines_list;
        lines_list << "line_1" << "line_2";

        foreach (const QString& line, lines_list)
        {
            if (obj.contains(line))
            {
                Text_t text_data;
                QJsonObject line_obj = obj[line].toObject();
                if (line_obj.contains("exposure"))
                    text_data.exposure = line_obj["exposure"].toDouble();
                text_data.text = line_obj["values"].toVariant().toStringList();

                if (line == "line_1")
                {
                    result.line1 = text_data;
                }
                else if (line == "line_2")
                {
                    result.line2 = text_data;
                }
            }
        }

        return result;
    }

    Audio_t fillAudioElement(const QJsonValue& audio_element_value) const
    {
        Audio_t audio_file;
        QJsonObject audio_object = audio_element_value.toObject();
        audio_file.name = audio_object["name"].toString();;
        audio_file.id = audio_object["id"].toInt();
        audio_file.sorting = audio_object["sorting"].toInt();
        audio_file.pause = audio_object["pause"].toDouble();
        audio_file.crc = audio_object["crcSHA256"].toString();
        audio_file.fileSize = audio_object["fileSize"].toInt();
        audio_file.url = audio_object["url"].toString();
        return audio_file;
    }

    QVector<Audio_t> fillAudioResources(const QJsonValue& value) const
    {
        QJsonArray audio_files = value.toObject()["resources"].toObject()["audio"].toArray();
        QVector<Audio_t> result;

        foreach (const QJsonValue& value, audio_files)
        {
            result.push_back(fillAudioElement(value));
        }

        return result;
    }

    QVector<Audio_t> fillExternalSpeakerResources(const QJsonValue& value) const
    {
        QJsonArray audio_files = value.toObject()["resources"].toObject()["externalSpeaker"].toArray();
        QVector<Audio_t> result;

        foreach (const QJsonValue& value, audio_files)
        {
            result.push_back(fillAudioElement(value));
        }

        return result;
    }

    QVector<Audio_t> fillStationAudioResources(const QJsonValue& station, const QString& resources_type) const
    {
        QJsonObject resources_obj = station.toObject()["resources"].toObject()["audio"].toObject();
        QJsonArray work_array;
        QVector<Audio_t> result;

        if (resources_type == "stop_out")
            work_array = resources_obj["stop_out"].toArray();
        else if (resources_type == "stop_in")
            work_array = resources_obj["stop_in"].toArray();
        else if (resources_type == "stop_over")
            work_array = resources_obj["stop_over"].toArray();

        foreach (const QJsonValue& value, work_array)
        {
            result.push_back(fillAudioElement(value));
        }

        return result;
    }

private:
    QJsonDocument m_json_doc;
};

#endif // ROUTESPARSER_H
