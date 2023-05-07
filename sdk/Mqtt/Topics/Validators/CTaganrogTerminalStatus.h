#pragma once

#include <Mqtt/Topics/CGrValidatorsTopic.h>
#include <QVector>

struct TerminalInfo
{
    QString id{"unknown"};
    bool shiftOpen{false};
};

class CTaganrogTerminalStatus : public CGrValidatorsTopic
{
public:
    explicit CTaganrogTerminalStatus();
    inline void append(const TerminalInfo& info){m_data.push_back(info);}
    inline void setData(const QVector<TerminalInfo>& data) {m_data = data;}
    inline QVector<TerminalInfo> getInfo() const {return m_data;}
    /*!
     * \brief isShiftOpen - if even one terminal have open shift, return true
     * otherwise return false
     */
    bool isShiftOpen() const;
protected:
    const QByteArray prepareData() const override;
    bool parseData(const QByteArray &data) override;
private:
    QVector<TerminalInfo> m_data;
};

