// Local
#include "CTopicTextForLed.h"

// SDK
#include "Mqtt/Topics/Led/TextRcHandlers.h"


static const char* TEXT_FOR_LED_TOPIC_NAME = "LED/TEXT_FOR";

CTopicTextForLed::CTopicTextForLed()
    : CMqttTopic(TEXT_FOR_LED_TOPIC_NAME)
    , m_msgType(TextMsg)
{}

TextRC_t CTopicTextForLed::ledNum() const
{
    return  m_ledNum;
}

TextRC_t CTopicTextForLed::ledSide() const
{
    return m_ledSide;
}

TextRC_t CTopicTextForLed::ledFront() const
{
    return m_ledFront;
}

TextRC_t CTopicTextForLed::ledInternal() const
{
    return m_ledInternal;
}

TextRC_t CTopicTextForLed::lcdText() const
{
    return m_lcdText;
}

CTopicTextForLed::TextForLedMsgType CTopicTextForLed::msgType() const
{
    return m_msgType;
}

void CTopicTextForLed::setLedNum(const TextRC_t& ledNum)
{
    m_ledNum = ledNum;
}

void CTopicTextForLed::setLedSide(const TextRC_t& ledSide)
{
    m_ledSide = ledSide;
}

void CTopicTextForLed::setLedFront(const TextRC_t& ledFront)
{
    m_ledFront = ledFront;
}

void CTopicTextForLed::setLedInternal(const TextRC_t& ledInternal)
{
    m_ledInternal = ledInternal;
}

void CTopicTextForLed::setLcdText(const TextRC_t& lcdText)
{
    m_lcdText = lcdText;
}

void CTopicTextForLed::setMsgType(CTopicTextForLed::TextForLedMsgType msgType)
{
    m_msgType = msgType;
}

const QByteArray CTopicTextForLed::prepareData() const
{
    QJsonObject object;

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

    QJsonObject lcdText_obj;
    TextRcHandlers::textRcToJson(lcdText_obj, m_lcdText);
    object.insert("lcd_text", lcdText_obj);

    QString msgType = TextForLedMsgTypeToStr(m_msgType);
    object.insert("msgType", msgType);

    QJsonDocument doc(object);
    return doc.toJson();
}

bool CTopicTextForLed::parseData(const QByteArray& data)
{
    QJsonDocument json_doc = QJsonDocument::fromJson(QString(data).toUtf8());

    m_ledNum = TextRC_t();
    m_ledSide = TextRC_t();
    m_ledFront = TextRC_t();
    m_ledInternal = TextRC_t();
    m_lcdText = TextRC_t();
    m_msgType = TextMsg;

    if (!json_doc.isNull())
    {
        QJsonObject json_obj = json_doc.object();

        if (json_obj.contains("led_num") && json_obj.contains("led_side") &&
            json_obj.contains("led_front") && json_obj.contains("lcd_text") && json_obj.contains("led_internal"))
        {
            QJsonObject ledNum_obj = json_obj["led_num"].toObject();
            TextRcHandlers::fillTextRC(ledNum_obj, m_ledNum);
            QJsonObject ledSide_obj = json_obj["led_side"].toObject();
            TextRcHandlers::fillTextRC(ledSide_obj, m_ledSide);
            QJsonObject ledFront_obj = json_obj["led_front"].toObject();
            TextRcHandlers::fillTextRC(ledFront_obj, m_ledFront);
            QJsonObject ledInternal_obj = json_obj["led_internal"].toObject();
            TextRcHandlers::fillTextRC(ledInternal_obj, m_ledInternal);
            QJsonObject lcdtext_obj = json_obj["lcd_text"].toObject();
            TextRcHandlers::fillTextRC(lcdtext_obj, m_lcdText);

            QString msgType = json_obj["msgType"].toString();
            m_msgType = TextForLedMsgTypeFromStr(msgType);

            return setValidState(true);
        }
    }

    return setValidState(false);
}

QString CTopicTextForLed::TextForLedMsgTypeToStr(CTopicTextForLed::TextForLedMsgType msgType) const
{
    if (msgType == EmergencyMsg)
    {
        return "EmergencyMsg";
    }
    else if (msgType == TextMsg)
    {
        return "TextMsg";
    }
    return "";
}

CTopicTextForLed::TextForLedMsgType CTopicTextForLed::TextForLedMsgTypeFromStr(const QString& str)
{
    if (str == "EmergencyMsg")
    {
        return EmergencyMsg;
    }
    else if (str == "TextMsg")
    {
        return TextMsg;
    }

    return TextMsg;
}
