#include "CTaganrogTerminalStatus.h"

#include <QDebug>

namespace
{
    const QString TERMINALS_FIELD   = "terminals";
    const QString ID_FIELD          = "id";
    const QString SHIFT_STATUS      = "shiftStatus";
    const QString TOPIC_NAME        = "TAGANROG";
}

CTaganrogTerminalStatus::CTaganrogTerminalStatus()
    : CGrValidatorsTopic(TOPIC_NAME)
{}

bool CTaganrogTerminalStatus::isShiftOpen() const
{
    for (const auto& terminal : m_data)
    {
        if (terminal.shiftOpen == true) return true;
    }

    return false;
}


const QByteArray CTaganrogTerminalStatus::prepareData() const
{
    QJsonObject mainObj;
    QJsonArray terminalArr;

    for (const auto& terminal : m_data)
    {
        QJsonObject terminalObj;
        terminalObj.insert(ID_FIELD, terminal.id);
        terminalObj.insert(SHIFT_STATUS, terminal.shiftOpen);
        terminalArr.append(terminalObj);
    }

    mainObj.insert(TERMINALS_FIELD, terminalArr);

    return QJsonDocument(mainObj).toJson();
}

bool CTaganrogTerminalStatus::parseData(const QByteArray &data)
{
    bool result{false};

    m_data.clear();

    QJsonObject obj = QJsonDocument::fromJson(data).object();

    if (obj.isEmpty())
    {
        printEmptyJsonMessage();
        return setValidState(result);
    }
    if (!obj.contains(TERMINALS_FIELD))
    {
        qWarning() << Q_FUNC_INFO << QString("%1 doesnt contain field %2")
                      .arg(topicName(), TERMINALS_FIELD);

        return setValidState(result);
    }

    QJsonArray terminalArr = obj.value(TERMINALS_FIELD).toArray();

    for (const auto& terminal : qAsConst(terminalArr))
    {
        QJsonObject terminalObj = terminal.toObject();
        TerminalInfo info;
        info.id = terminalObj.value(ID_FIELD).toString();
        info.shiftOpen = terminalObj.value(SHIFT_STATUS).toBool();
        m_data.append(info);
    }

    result = true;

    return setValidState(result);
}
