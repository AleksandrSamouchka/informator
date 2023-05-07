//Local
#include "usbchargetopic.h"

//STD
#include <cmath>

//Qt
#include <QDebug>

namespace
{
    const QString   USB_CHARGE_TOPIC_NAME     = "LIAZ/USB_CHARGE/";

    const int       SOCKETS_COUNT             = 3;

    // Ключи для полей топика
    const QString   LOGIC_ADDR_FILED          = "logicAddr";
    const QString   SOCKETS_ERRORS_FIELD      = "socketsError";
    const QString   SOCKETS_VOLTAGE_FIELD     = "socketsVoltage";
    const QString   SUMMARY_POWER_FIELD       = "summaryPower";

    const double    VOLTAGE_FACTOR            = 0.025;
    const double    CURRENT_FACTOR            = 0.01;

    double toVoltage(double input)
    {
        return std::round((input*VOLTAGE_FACTOR)*10)/10;
    };

    double toCurrent(double input)
    {
        return std::round((input*CURRENT_FACTOR)*100)/100;
    };
}

UsbChargeTopic::UsbChargeTopic(const QString &usbID)
    : CMqttTopic(USB_CHARGE_TOPIC_NAME + usbID)
{
    m_USBData.socketsVoltages.resize(SOCKETS_COUNT);
    m_USBData.socketsCurrents.resize(SOCKETS_COUNT);
    m_USBData.socketsErrors.resize(SOCKETS_COUNT);
}

void UsbChargeTopic::setLogicAddr(int logicAddr)
{
    m_USBData.id = logicAddr;
}

void UsbChargeTopic::setSocketsVoltages(const QVector<double> &socketsVoltages)
{
    for (int i = 0; i < socketsVoltages.size(); ++i)
    {
        m_USBData.socketsVoltages[i] = toVoltage(socketsVoltages[i]);
    }
}

void UsbChargeTopic::setSocketsCurrents(const QVector<double> &socketsCurrents)
{
    for (int i = 0; i < socketsCurrents.size(); ++i)
    {
        m_USBData.socketsCurrents[i] = toCurrent(socketsCurrents[i]);
    }
}

void UsbChargeTopic::setSocketsErrors(const QVector<bool> &socketsErrors)
{
    m_USBData.socketsErrors = socketsErrors;
}

USBData UsbChargeTopic::getUSBData() const
{
    return m_USBData;
}

const QByteArray UsbChargeTopic::prepareData() const
{
    QJsonObject jsonObj;

    jsonObj.insert(LOGIC_ADDR_FILED, m_USBData.id);

    QJsonArray socketsVoltagesArr;

    for (const auto& voltage : m_USBData.socketsVoltages)
    {
        socketsVoltagesArr.append(voltage);
    }

    jsonObj.insert(SOCKETS_VOLTAGE_FIELD, socketsVoltagesArr);

    QJsonArray socketsErrorsArr;

    for (const auto& isError : m_USBData.socketsErrors)
    {
        socketsErrorsArr.append(isError);
    }

    jsonObj.insert(SOCKETS_ERRORS_FIELD, socketsErrorsArr);

    calcSummaryPower();

    jsonObj.insert(SUMMARY_POWER_FIELD, m_USBData.summaryPower);

    return QJsonDocument(jsonObj).toJson();
}

bool UsbChargeTopic::parseData(const QByteArray &data)
{
    bool result = false;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if (!jsonDoc.isEmpty())
    {
        QJsonObject jsonMainObj = jsonDoc.object();

        if (jsonMainObj.contains(LOGIC_ADDR_FILED))
        {
            m_USBData.id = jsonMainObj[LOGIC_ADDR_FILED].toInt();

            QJsonArray socketsVoltages = jsonMainObj[SOCKETS_VOLTAGE_FIELD].toArray();

            for (int i = 0; i < socketsVoltages.size(); ++i)
            {
                m_USBData.socketsVoltages[i] = socketsVoltages[i].toDouble();
            }

            QJsonArray socketsErrors = jsonMainObj[SOCKETS_ERRORS_FIELD].toArray();

            for (int i = 0; i < socketsErrors.size(); ++i)
            {
                m_USBData.socketsErrors[i] = socketsErrors[i].toVariant().toBool();
            }

            m_USBData.summaryPower = jsonMainObj[SUMMARY_POWER_FIELD].toDouble();

            result = true;
        }
        else
        {
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else
    {
        printEmptyJsonMessage();
    }

    return setValidState(result);
}

void UsbChargeTopic::calcSummaryPower() const
{
    for (int i = 0; i < SOCKETS_COUNT; ++i)
    {
        m_USBData.summaryPower += m_USBData.socketsVoltages[i] * m_USBData.socketsCurrents[i];
    }

    m_USBData.summaryPower = std::floor(m_USBData.summaryPower*10)/10;
}

