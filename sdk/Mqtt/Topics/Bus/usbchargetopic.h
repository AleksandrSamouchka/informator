#pragma once

//SDK
#include <Mqtt/CMqttTopic.h>

//Qt
#include <QVector>

struct USBData
{
    int             id{-1};
    QVector<double> socketsVoltages{0, 0, 0};
    QVector<double> socketsCurrents{0, 0, 0};
    QVector<bool>   socketsErrors{0, 0, 0};
    double          summaryPower{0};
};

class UsbChargeTopic : public CMqttTopic
{
public:
    explicit UsbChargeTopic(const QString& usbID);
    void            setLogicAddr(int logicAddr);
    void            setSocketsVoltages(const QVector<double>& socketsVoltages);
    void            setSocketsCurrents(const QVector<double>& socketsCurrents);
    void            setSocketsErrors(const QVector<bool>& socketsErrors);
    USBData         getUSBData() const;

protected:
    const QByteArray    prepareData() const override;
    bool                parseData(const QByteArray& data) override;
private:
    void calcSummaryPower() const;
private:
    mutable USBData m_USBData;
};
