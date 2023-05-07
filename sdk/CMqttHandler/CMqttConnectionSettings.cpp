#include "CMqttConnectionSettings.h"
#include "CMqttClientHandler.h"
#include "CSettings.h"
#include <QtNetwork/QHostAddress>

const QString CMqttConnectionSettings::MQTTSETT_GROUP = "";
const QString CMqttConnectionSettings::MQTTSETT_IP = "MqttBrokerIp";
const QString CMqttConnectionSettings::MQTTSETT_PORT = "MqttBrokerPort";

/** ***********************************************************************
 * @fn CMqttConnectionSettings  [конструктор]
 * @brief метод, инициализирует настройки подключения к брокеру Mqtt
 ** ***********************************************************************/
CMqttConnectionSettings::CMqttConnectionSettings()
{
    const QString localhost{ QHostAddress{ QHostAddress::LocalHost }.toString() };
    m_ip = CSettings::readValue( MQTTSETT_GROUP + MQTTSETT_IP, localhost ).toString();
    m_port = CSettings::readValue( MQTTSETT_GROUP + MQTTSETT_PORT, 1883 ).toUInt();
}

/** ***********************************************************************
 * @fn CMqttConnectionSettings  [конструктор]
 * @brief метод, использует пользовательские настройки подключения к
 * брокеру Mqtt
 ** ***********************************************************************/
CMqttConnectionSettings::CMqttConnectionSettings(const QString &ip,
        const uint16_t port):
        m_ip(ip),
        m_port(port)
{

}

/** ***********************************************************************
 * @fn setters
 * @brief слоты для задания свойств класса
 ** ***********************************************************************/
void CMqttConnectionSettings::setIp(const QString &ip)
{
    m_ip = ip;
}

void CMqttConnectionSettings::setPort(uint16_t port)
{
    m_port = port;
}

/** ***********************************************************************
 * @fn isValid
 * @brief метод проверки корректности введенных данных
 * @param none
 * @return none
 ** ***********************************************************************/
bool CMqttConnectionSettings::isValid() const
{
    bool res{ false };
    m_lastError.clear();
    if ( QHostAddress( m_ip ).isNull() )
    {
        m_lastError = "ОШИБКА: некорректный IP адрес";
    }
    else if ( m_port == 0 )
    {
        m_lastError = "ОШИБКА: нулевой порт";
    }
    else
    {
        res = true;
    }
    return res;
}

/** ***********************************************************************
 * @fn getters
 * @brief методы для чтения свойств класса
 ** ***********************************************************************/
QString CMqttConnectionSettings::getIp() const
{
    return m_ip;
}

uint16_t CMqttConnectionSettings::getPort() const
{
    return m_port;
}

/** ***********************************************************************
 * @fn getLastError
 * @brief метод для получения текста последней ошибки.
 * Обновляется при выполнении проверки параметров подключения
 * @param none
 * @return QString
 ** ***********************************************************************/
QString CMqttConnectionSettings::getLastError() const
{
    return m_lastError;
}
