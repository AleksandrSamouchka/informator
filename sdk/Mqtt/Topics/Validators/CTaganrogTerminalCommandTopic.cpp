#include "CTaganrogTerminalCommandTopic.h"

#include <QHash>
#include <QDebug>

namespace
{
    const QString TOPIC_NAME = "TAGANROG_TASK";
    const QString TASK_FIELD = "task";
    const QString ID_FIELD   = "id";

    // NOTE: this string command must be equal to command in mntconti_ii2
    const QHash<Task, QString> TASK_TO_STRING
    {
        {openShift, "OPEN_SHIFT"},
        {closeShift, "CLOSE_SHIFT"},
        {synchronize, "SYNC"},
        {reboot, "REBOOT"}
    };

    const QHash<QString, Task> TASK_TO_ENUM
    {
        {"OPEN_SHIFT", openShift},
        {"CLOSE_SHIFT", closeShift},
        {"SYNC", synchronize},
        {"REBOOT", reboot}
    };
}

CTaganrogTerminalCommandTopic::CTaganrogTerminalCommandTopic()
    : CGrValidatorsTopic(TOPIC_NAME)
{}


const QByteArray CTaganrogTerminalCommandTopic::prepareData() const
{
    QJsonObject obj;
    obj.insert(TASK_FIELD, TASK_TO_STRING.value(m_task));
    obj.insert(ID_FIELD, m_id);
    return QJsonDocument(obj).toJson();
}

bool CTaganrogTerminalCommandTopic::parseData(const QByteArray &data)
{
    QJsonObject obj = QJsonDocument::fromJson(data).object();

    if (obj.empty())
    {
        qWarning() << Q_FUNC_INFO << "Empty incomnig data";
        return setValidState(false);
    }
    if (!obj.contains(TASK_FIELD))
    {
        qWarning() << Q_FUNC_INFO << "Incoming data doesnt contain" << TASK_FIELD;
        return setValidState(false);
    }

    m_task = TASK_TO_ENUM.value(obj.value(TASK_FIELD).toString());

    if (obj.contains(ID_FIELD))
    {
        m_id = obj.value(ID_FIELD).toString();
    }

    return setValidState(true);
}

QString CTaganrogTerminalCommandTopic::getTask() const
{
    return TASK_TO_STRING.value(m_task);
}
