#include "CDatabaseConnectionSettings.h"
#include <CSettings/CSettings.h>
#include <CUtils/CUtils.h>
#include <QtNetwork/QHostAddress>

static const char *DBSETT_GROUP = "";
static const char *DBSETT_IP = "DatabaseIp";
static const char *DBSETT_PORT = "DatabasePort";
static const char *DBSETT_USER = "DatabaseUser";
static const char *DBSETT_PASSWORD = "DatabasePassword";

static const char *localhost = "localhost"; 


/** ***********************************************************************
 * @fn CDatabaseConnectionSettings  [конструктор]
 * @brief метод, инициализирует настройки подключения к БД из файла настроек
 ** ***********************************************************************/
CDatabaseConnectionSettings::CDatabaseConnectionSettings()
{
    QString group(DBSETT_GROUP);
    m_ip = CSettings::readValue( /*DBSETT_GROUP*/group + DBSETT_IP, /*CUtils::*/localhost ).toString();
    m_port = static_cast< uint16_t >( CSettings::readValue( /*DBSETT_GROUP*/group + DBSETT_PORT, 3306 ).toUInt() );
    m_user = CSettings::readValue( /*DBSETT_GROUP*/group + DBSETT_USER,  "mntconti" ).toString();
    m_password = CSettings::readValue( /*DBSETT_GROUP*/group + DBSETT_PASSWORD,  "18iTnOcTnM81" ).toString();
}

/** ***********************************************************************
 * @fn setters
 * @brief слоты для задания свойств класса
 ** ***********************************************************************/
void CDatabaseConnectionSettings::setIp(const QString &ip)
{
    m_ip = ip;
}

void CDatabaseConnectionSettings::setPort(const uint16_t port)
{
    m_port = port;
}

void CDatabaseConnectionSettings::setUser(const QString &user)
{
    m_user = user;
}

void CDatabaseConnectionSettings::setPassword(const QString &password)
{
    m_password = password;
}

/** ***********************************************************************
 * @fn isValid
 * @brief метод проверки корректности введенных данных
 * @param none
 * @return none
 ** ***********************************************************************/
bool CDatabaseConnectionSettings::isValid()
{
    bool res{ false };
    m_lastError.clear();
    if ( m_ip != /*CUtils::*/localhost && QHostAddress( m_ip ).isNull() )
    {
        m_lastError = "ОШИБКА: некорректный IP адрес";
    }
    else if ( m_port == 0 )
    {
        m_lastError = "ОШИБКА: нулевой порт";
    }
    else if ( m_user.isEmpty() )
    {
        m_lastError = "ОШИБКА: пустое имя пользователя";
    }
    else if ( m_password.isEmpty() )
    {
        m_lastError = "ОШИБКА: не указан пароль доступа";
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
QString CDatabaseConnectionSettings::getIp() const
{
    return m_ip;
}

uint16_t CDatabaseConnectionSettings::getPort() const
{
    return m_port;
}

QString CDatabaseConnectionSettings::getUser() const
{
    return m_user;
}

QString CDatabaseConnectionSettings::getPassword() const
{
    return m_password;
}

/** ***********************************************************************
 * @fn getLastError
 * @brief метод для получения текста последней ошибки.
 * Обновляется при выполнении проверки параметров подключения
 * @param none
 * @return QString
 ** ***********************************************************************/
QString CDatabaseConnectionSettings::getLastError() const
{
    return m_lastError;
}
