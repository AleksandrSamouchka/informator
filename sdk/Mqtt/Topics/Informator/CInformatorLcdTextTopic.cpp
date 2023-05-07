// Local
#include "CInformatorLcdTextTopic.h"
#include "../Led/TextRcHandlers.h"


static const char* INFORMATOR_LCDTEXT_TOPIC_NAME = "INFORMATOR/LCDTEXT";


CInformatorLcdTextTopic::CInformatorLcdTextTopic()
    : CMqttTopic(INFORMATOR_LCDTEXT_TOPIC_NAME)
{}

void CInformatorLcdTextTopic::setLcdText(const TextRC_t& lcdText)
{
    m_lcdText = lcdText;
}

TextRC_t CInformatorLcdTextTopic::lcdText() const
{
    return m_lcdText;
}

const QByteArray CInformatorLcdTextTopic::prepareData() const
{
    QJsonObject object;

    QJsonObject lcdText_obj;
    TextRcHandlers::textRcToJson(lcdText_obj, m_lcdText);
    object.insert("lcd_text", lcdText_obj);

    QJsonDocument doc(object);
    return doc.toJson();
}

bool CInformatorLcdTextTopic::parseData(const QByteArray& data)
{
    QJsonDocument json_doc = QJsonDocument::fromJson(QString(data).toUtf8());
    m_lcdText = TextRC_t();

    if (!json_doc.isNull())
    {
        QJsonObject json_obj = json_doc.object();

        if (json_obj.contains("lcd_text"))
        {
            QJsonObject lcdText_obj = json_obj["lcd_text"].toObject();
            TextRcHandlers::fillTextRC(lcdText_obj, m_lcdText);
            return setValidState(true);
        }
    }

    return setValidState(false);
}
