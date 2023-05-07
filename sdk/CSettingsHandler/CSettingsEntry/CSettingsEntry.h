#ifndef CSETTINGSENTRY_H
#define CSETTINGSENTRY_H

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>

/** ***********************************************************************
 * @class CSettingsEntry
 * @brief класс с описанием настройки приложения (модуля), которая хранится в БД
 ** ***********************************************************************/
class CSettingsEntry
{
public:
    enum EApplicationSettingsTypes
    {
        SettingsType_text = 0,
        SettingsType_number,
    };

public:
    CSettingsEntry() = default;
    CSettingsEntry( const CSettingsEntry & other ) = default;

    explicit CSettingsEntry( const QString &name,
                             const QString &humanName,
                             const QString &value,
                             const QString &type = QString{},
                             const QString &description = QString{},
                             const QString &module = QCoreApplication::applicationName(),
                             const bool isPrivate = false,
                             const QString &subgroupName = QString{},
                             const QString &subgroupHumanName = QString{} );

/*! Конструктор для приватной настройки */
    explicit CSettingsEntry( const QString &name,
                             const QString &value,
                             const bool isPrivate = true,
                             const QString &subgroupName = QString{},
                             const QString &subgroupHumanName = QString{} );

    QString getName() const;
    QString getHumanName() const;
    QString getValue() const;
    QString getType() const;
    QString getModule() const;
    QString getDescription() const;
    bool isPrivate() const;
    QString getSubgroupName() const;
    QString getSubgroupHumanName() const;

    bool isValid() const;

    static QString getTypeString( const EApplicationSettingsTypes type );

public slots:
    void setName                ( const QString &name );
    void setHumanName           ( const QString &humanName );
    void setValue               ( const QString &value );
    void setType                ( const QString &type );
    void setModule              ( const QString &module );
    void setDescription         ( const QString &description );
    void setPrivate             ( const bool isPrivate );
    void setSubgroupName        ( const QString &name );
    void setSubgroupHumanName   ( const QString &humanName );

private:
    QString m_name = QString{};
    QString m_humanName = QString{};
    QString m_value = QString{};
    QString m_type = QString{};
    QString m_module = QCoreApplication::applicationName();
    QString m_description = QString{};
    bool m_private = false;
    QString m_subgroupName{};
    QString m_subgroupHumanName{};
};

#endif // CSETTINGSENTRY_H
