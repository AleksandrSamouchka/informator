#ifndef CAPPLICATIONSETTINGS_H
#define CAPPLICATIONSETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

/** ***********************************************************************
 * @namespace DB_SETT
 * @brief пространство имен, содержит константы для работы с настройками,
 * хранящимися в БД
 ** ***********************************************************************/
namespace DB_SETT
{
    enum EApplicationSettingsTypes
    {
        SettingsType_text = 0,
        SettingsType_number,
    };
    QString getTypeString( const EApplicationSettingsTypes type );
}

/** ***********************************************************************
 * @class CApplicationSettings
 * @brief класс с описанием настройки приложения (модуля), которая хранится в БД
 ** ***********************************************************************/
class CApplicationSettings
{
    QString m_parameter;
    QString m_humanName;
    QVariant m_value;
    QString m_type;

public slots:
    void setParameter( const QString &parameter );
    void setHumanName( const QString &humanName );
    void setValue( const QVariant &value );
    void setType( const QString &type );

public:
    CApplicationSettings() = default;
    CApplicationSettings( const QString &parameter,
                          const QString &humanName,
                          const QVariant &value,
                          const QString &type );

    QString getParameter() const;
    QString getHumanName() const;
    QVariant getValue() const;
    QString getType() const;

    bool isValid() const;
    bool isParameter( const QString &parameter ) const;
};

#endif // CAPPLICATIONSETTINGS_H
