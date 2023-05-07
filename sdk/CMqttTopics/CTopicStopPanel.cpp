// Qt
#include <QHash>

// Local
#include "CTopicStopPanel.h"

static const char* STOP_PANEL_TOPIC_NAME = "LED/STOP_PANEL_TEXT";


CTopicStopPanel::CTopicStopPanel()
    : CMqttTopic(STOP_PANEL_TOPIC_NAME)
{}

CTopicStopPanel::STOP_PANEL_ACTIONS CTopicStopPanel::action() const
{
    return m_action;
}

void CTopicStopPanel::setAction(CTopicStopPanel::STOP_PANEL_ACTIONS act)
{
    m_action = act;
}

const QByteArray CTopicStopPanel::prepareData() const
{
    QJsonObject object;
    object.insert("action", actionToString(m_action));
    QJsonDocument doc(object);
    return doc.toJson();
}

bool CTopicStopPanel::parseData(const QByteArray& data)
{
    m_action = STOP_PANEL_ACTIONS::UNKNOWN;
    QJsonDocument json_doc = QJsonDocument::fromJson(QString(data).toUtf8());

    if (!json_doc.isNull())
    {
        QJsonObject json_obj = json_doc.object();

        if (!json_obj.isEmpty())
        {
            if (json_obj.contains("action"))
            {
                QString action = json_obj["action"].toString();
                m_action = stringToAction(action);
                return setValidState(true);
            }
        }
    }

    return setValidState(false);
}

QString CTopicStopPanel::actionToString(CTopicStopPanel::STOP_PANEL_ACTIONS act) const
{
    QHash<CTopicStopPanel::STOP_PANEL_ACTIONS, QString> convertHash;
    convertHash[STOP_PANEL_ACTIONS::UNKNOWN] = "UNKNOWN";
    convertHash[STOP_PANEL_ACTIONS::BLINK_STOP] = "BLINK_STOP";
    convertHash[STOP_PANEL_ACTIONS::PASSANGER] = "PASSANGER";
    convertHash[STOP_PANEL_ACTIONS::THANKS] = "THANKS";
    return convertHash[act];
}

CTopicStopPanel::STOP_PANEL_ACTIONS CTopicStopPanel::stringToAction(const QString &act) const
{
    QHash<QString, CTopicStopPanel::STOP_PANEL_ACTIONS> convertHash;
    convertHash["UNKNOWN"] = STOP_PANEL_ACTIONS::UNKNOWN;
    convertHash["BLINK_STOP"] = STOP_PANEL_ACTIONS::BLINK_STOP;
    convertHash["PASSANGER"] = STOP_PANEL_ACTIONS::PASSANGER;
    convertHash["THANKS"] = STOP_PANEL_ACTIONS::THANKS;
    return convertHash[act];
}
