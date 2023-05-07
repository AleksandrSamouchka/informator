#pragma once

#include <Mqtt/Topics/CGrValidatorsTopic.h>

enum Task : int
{
    openShift = 0,
    closeShift,
    synchronize,
    reboot
};

class CTaganrogTerminalCommandTopic : public CGrValidatorsTopic
{
public:
    CTaganrogTerminalCommandTopic();
    inline void setTask(const Task& task){m_task = task;}
    QString     getTask() const;

    inline void    setID(const QString& id) { m_id = id;   }
    inline QString getID() const            { return m_id; }

protected:
    const QByteArray prepareData() const override;
    bool parseData(const QByteArray &data) override;
private:
    Task    m_task;
    QString m_id;
};

