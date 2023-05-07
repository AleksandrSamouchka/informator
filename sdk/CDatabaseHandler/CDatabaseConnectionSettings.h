#ifndef CDATABASECONNECTIONSETTINGS_H
#define CDATABASECONNECTIONSETTINGS_H

#include <QtCore/QObject>

/** ***********************************************************************
 * @class CDatabaseConnectionSettings
 * @brief класс с параметрами подключения к серверу БД MySQL
 ** ***********************************************************************/
class CDatabaseConnectionSettings
{
    QString m_ip;
    uint16_t m_port = 0;
    QString m_user;
    QString m_password;
    QString m_lastError;

public:
/*! Поля настроек из файла, которые относятся к БД */
//    static const QString DBSETT_GROUP;
//    static const QString DBSETT_IP;
//    static const QString DBSETT_PORT;
//    static const QString DBSETT_USER;
//    static const QString DBSETT_PASSWORD;

protected slots:
    void setIp( const QString &ip );
    void setPort( const uint16_t port );
    void setUser( const QString &user );
    void setPassword( const QString &password );

public:
    CDatabaseConnectionSettings();
    CDatabaseConnectionSettings( const CDatabaseConnectionSettings & other ) = default;

    bool isValid( );

    QString getIp( ) const;
    uint16_t getPort( ) const;
    QString getUser( ) const;
    QString getPassword( ) const;

    QString getLastError( ) const;
};

#endif // CDATABASECONNECTIONSETTINGS_H
