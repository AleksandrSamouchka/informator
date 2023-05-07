// Local
#include "CLedSettingsAndRouteInfoTopic.h"
#include "TextRcHandlers.h"

static const char* LED_SETTINGS_AND_ROUTEINFO_TOPIC_NAME = "LED/SETTINGS_AND_ROUTE_INFO";

CLedSettingsAndRouteInfoTopic::CLedSettingsAndRouteInfoTopic()
    : CMqttTopic(LED_SETTINGS_AND_ROUTEINFO_TOPIC_NAME)
{}

QString CLedSettingsAndRouteInfoTopic::routeNumber() const
{
    return m_routeNumber;
}

TextRC_t CLedSettingsAndRouteInfoTopic::ledNum() const
{
    return m_ledNum;
}

TextRC_t CLedSettingsAndRouteInfoTopic::ledSide() const
{
    return m_ledSide;
}

TextRC_t CLedSettingsAndRouteInfoTopic::ledFront() const
{
    return m_ledFront;
}

TextRC_t CLedSettingsAndRouteInfoTopic::ledInternal() const
{
    return m_ledInternal;
}

LedSettings_t CLedSettingsAndRouteInfoTopic::LedSettings() const
{
    return m_ledSettings;
}

void CLedSettingsAndRouteInfoTopic::setRouteNumber(const QString& routeNumber)
{
    m_routeNumber = routeNumber;
}

void CLedSettingsAndRouteInfoTopic::setLedNum(const TextRC_t& ledNum)
{
    m_ledNum = ledNum;
}

void CLedSettingsAndRouteInfoTopic::setLedSide(const TextRC_t& ledSide)
{
    m_ledSide = ledSide;
}

void CLedSettingsAndRouteInfoTopic::setLedFront(const TextRC_t& ledFront)
{
    m_ledFront = ledFront;
}

void CLedSettingsAndRouteInfoTopic::setLedInternal(const TextRC_t &ledInternal)
{
    m_ledInternal = ledInternal;
}

void CLedSettingsAndRouteInfoTopic::setLedSettings(const LedSettings_t& settings)
{
    m_ledSettings = settings;
}

const QByteArray CLedSettingsAndRouteInfoTopic::prepareData() const
{
    QJsonObject object;
    object.insert("routeNumber", m_routeNumber);

    QJsonObject ledNum_obj;
    TextRcHandlers::textRcToJson(ledNum_obj, m_ledNum);
    object.insert("led_num", ledNum_obj);

    QJsonObject ledSide_obj;
    TextRcHandlers::textRcToJson(ledSide_obj, m_ledSide);
    object.insert("led_side", ledSide_obj);

    QJsonObject ledFront_obj;
    TextRcHandlers::textRcToJson(ledFront_obj, m_ledFront);
    object.insert("led_front", ledFront_obj);

    QJsonObject ledInternal_obj;
    TextRcHandlers::textRcToJson(ledInternal_obj, m_ledInternal);
    object.insert("led_internal", ledInternal_obj);

    QJsonObject leSettings_obj;
    ledSettingsToJson(leSettings_obj, m_ledSettings);
    object.insert("ledPanelSettings", leSettings_obj);

    QJsonDocument doc(object);
    return doc.toJson();
}

bool CLedSettingsAndRouteInfoTopic::parseData(const QByteArray& data)
{
    QJsonDocument json_doc = QJsonDocument::fromJson(QString(data).toUtf8());
    m_routeNumber = "";
    m_ledNum = TextRC_t();
    m_ledSide = TextRC_t();
    m_ledFront = TextRC_t();
    m_ledInternal = TextRC_t();

    if (!json_doc.isNull())
    {
        QJsonObject json_obj = json_doc.object();

        if (json_obj.contains("routeNumber") && json_obj.contains("led_num") && json_obj.contains("led_side") &&
            json_obj.contains("led_front") && json_obj.contains("ledPanelSettings") && json_obj.contains("led_internal"))
        {         
            m_routeNumber = json_obj["routeNumber"].toString();
            QJsonObject ledNum_obj = json_obj["led_num"].toObject();
            TextRcHandlers::fillTextRC(ledNum_obj, m_ledNum);
            QJsonObject ledSide_obj = json_obj["led_side"].toObject();
            TextRcHandlers::fillTextRC(ledSide_obj, m_ledSide);
            QJsonObject ledFront_obj = json_obj["led_front"].toObject();
            TextRcHandlers::fillTextRC(ledFront_obj, m_ledFront);
            QJsonObject ledInternal_obj = json_obj["led_internal"].toObject();
            TextRcHandlers::fillTextRC(ledInternal_obj, m_ledInternal);
            QJsonObject ledSettings_obj = json_obj["ledPanelSettings"].toObject();
            fillLedSettings(ledSettings_obj, m_ledSettings);
            return setValidState(true);
        }
    }

    return setValidState(false);
}

void CLedSettingsAndRouteInfoTopic::fillLedSettings(const QJsonObject& obj, LedSettings_t& ledSettings)
{
    ledSettings.frontPanelHAlign = obj["frontPanelHAlign"].toString();
    ledSettings.sidePanelHAlign = obj["sidePanelHAlign"].toString();
    ledSettings.salonPanelHAlign = obj["salonPanelHAlign"].toString();

    ledSettings.routeNumberColorText = obj["routeNumberColorText"].toString();
    ledSettings.routeNumberColorBackGround = obj["routeNumberColorBackGround"].toString();
    ledSettings.textNumberColorText = obj["textNumberColorText"].toString();
    ledSettings.textNumberColorBackGround = obj["textNumberColorBackGround"].toString();
}

void CLedSettingsAndRouteInfoTopic::ledSettingsToJson(QJsonObject& obj, const LedSettings_t& m_ledSettings) const
{
    obj["frontPanelHAlign"] = m_ledSettings.frontPanelHAlign;
    obj["sidePanelHAlign"] = m_ledSettings.sidePanelHAlign;
    obj["salonPanelHAlign"] = m_ledSettings.salonPanelHAlign;
    obj["routeNumberColorText"] = m_ledSettings.routeNumberColorText;
    obj["routeNumberColorBackGround"] = m_ledSettings.routeNumberColorBackGround;
    obj["textNumberColorText"] = m_ledSettings.textNumberColorText;
    obj["textNumberColorBackGround"] = m_ledSettings.textNumberColorBackGround;
}
