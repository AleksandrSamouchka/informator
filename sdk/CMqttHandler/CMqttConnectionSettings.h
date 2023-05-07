#ifndef CMQTTCONNECTIONSETTINGS_H
#define CMQTTCONNECTIONSETTINGS_H


#include <QtCore/QObject>

/** ***********************************************************************
 * @class CMqttConnectionSettings
 * @brief класс с параметрами подключения к брокеру Mqtt
 ** ***********************************************************************/
class CMqttConnectionSettings
{
    QString m_ip = QString{};
    uint16_t m_port = 0;
    mutable QString m_lastError = QString{};

public:
    static const QString MQTTSETT_GROUP;
    static const QString MQTTSETT_IP;
    static const QString MQTTSETT_PORT;

public slots:
    void setIp( const QString &ip );
    void setPort( uint16_t port );

public:
    CMqttConnectionSettings();
    CMqttConnectionSettings(const QString &ip, const uint16_t port);
    CMqttConnectionSettings( const CMqttConnectionSettings & other ) = default;

    bool isValid( ) const;

    QString getIp( ) const;
    uint16_t getPort( ) const;
    QString getLastError( ) const;
};

#endif // CMQTTCONNECTIONSETTINGS_H
