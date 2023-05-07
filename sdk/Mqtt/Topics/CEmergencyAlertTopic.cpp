// Local
#include "CEmergencyAlertTopic.h"
#include "Led/TextRcHandlers.h"


static const char* EMERGENCY_ALERT_TOPIC_NAME = "EMERGENCY_ALERT/";

CEmergencyAlertTopic::CEmergencyAlertTopic(const QString& depot_id)
    : CMqttTopic(EMERGENCY_ALERT_TOPIC_NAME + depot_id)
    , m_isPeriodic(false)
    , m_period(0)
    , m_id(0)
    , m_msgType(MsgType::StopMsg)
{}

int CEmergencyAlertTopic::period() const
{
    return m_period;
}

int& CEmergencyAlertTopic::period()
{
    return m_period;
}

bool CEmergencyAlertTopic::isPeriodic() const
{
    return m_isPeriodic;
}

unsigned int CEmergencyAlertTopic::id() const
{
    return m_id;
}

CEmergencyAlertTopic::MsgType CEmergencyAlertTopic::msgType() const
{
    return m_msgType;
}

QVector<Audio_t> CEmergencyAlertTopic::audioFiles() const
{
    return m_audioFiles;
}

TextRC_t CEmergencyAlertTopic::ledNum() const
{
    return m_ledNum;
}

TextRC_t CEmergencyAlertTopic::ledSide() const
{
    return m_ledSide;
}

TextRC_t CEmergencyAlertTopic::ledFront() const
{
    return m_ledFront;
}

TextRC_t CEmergencyAlertTopic::ledInternal() const
{
    return m_ledInternal;
}

TextRC_t CEmergencyAlertTopic::lcdText() const
{
    return  m_lcdText;
}

const QByteArray CEmergencyAlertTopic::prepareData() const
{
    return QByteArray();
}

bool CEmergencyAlertTopic::parseData(const QByteArray& data)
{
    bool playFinished = isPeriodic() && m_period == 0;

    if ((m_rawData != data) || playFinished)
    {
        m_rawData = data;

        m_isPeriodic = false;
        m_period = 0;

        m_ledNum = TextRC_t();
        m_ledSide = TextRC_t();
        m_ledFront = TextRC_t();
        m_ledInternal = TextRC_t();
        m_lcdText = TextRC_t();

        m_audioFiles.clear();
        m_msgType = MsgType::StopMsg;

        QJsonDocument json_doc = QJsonDocument::fromJson(QString(data).toUtf8());

        if (!json_doc.isNull())
        {
            QJsonObject json_obj = json_doc.object();

            if (!json_obj.isEmpty())
            {
                if (json_obj.contains("id") && (!json_obj.contains("isPeriodic") && !json_obj.contains("period") &&
                                                !json_obj.contains("audio") && !json_obj.contains("text")))
                {
                    m_msgType = MsgType::StopMsg;
                    m_id = json_obj["id"].toVariant().toUInt();
                    return setValidState(true);
                }
                else if (json_obj.contains("isPeriodic") && json_obj.contains("period") &&
                         json_obj.contains("audio") && json_obj.contains("text"))
                {
                    m_id = json_obj["id"].toVariant().toUInt();
                    m_msgType = MsgType::PlayMsg;
                    m_isPeriodic = json_obj["isPeriodic"].toBool();
                    m_period = json_obj["period"].toInt();

                    QJsonArray audio_files = json_obj["audio"].toArray();
                    for (auto it = audio_files.begin(); it != audio_files.end(); ++it)
                    {
                        Audio_t audio_file;
                        QJsonObject audio_object = (*it).toObject();
                        audio_file.id = audio_object["id"].toInt();
                        audio_file.name = audio_object["name"].toString();
                        audio_file.url = audio_object["url"].toString();
                        audio_file.pause = audio_object["pause"].toInt();
                        m_audioFiles.push_back(audio_file);
                    }

                    QJsonObject json_text_obj = json_obj["text"].toObject();
                    QJsonObject ledNum_obj = json_text_obj["led_num"].toObject();
                    TextRcHandlers::fillTextRC(ledNum_obj, m_ledNum);
                    QJsonObject ledSide_obj = json_text_obj["led_side"].toObject();
                    TextRcHandlers::fillTextRC(ledSide_obj, m_ledSide);
                    QJsonObject ledFront_obj = json_text_obj["led_front"].toObject();
                    TextRcHandlers::fillTextRC(ledFront_obj, m_ledFront);
                    QJsonObject ledInternal_obj = json_text_obj["led_internal"].toObject();
                    TextRcHandlers::fillTextRC(ledInternal_obj, m_ledInternal);
                    QJsonObject lcdText_obj = json_text_obj["lcd_text"].toObject();
                    TextRcHandlers::fillTextRC(lcdText_obj, m_lcdText);

                    return setValidState(true);
                }
            }
        }
        return setValidState(false);
    }

    return setValidState(true);
}
