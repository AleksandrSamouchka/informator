#ifndef CSETTINGSDATABASEPROVIDER_H
#define CSETTINGSDATABASEPROVIDER_H


#include <CSettingsHandler/CAbstractSettingsProvider.h>
#include <CDatabaseHandler/CDatabaseHandler.h>

#include <QtCore/QPointer>
#include <QtCore/QCoreApplication>

/** ***********************************************************************
 * @class CSettingsDatabaseProvider
 * @brief Класс одиночка (singleton). Обработчик методов работы с настройками в БД.
 ** ***********************************************************************/
class CSettingsDatabaseProvider : public CAbstractSettingsProvider
{
    Q_OBJECT
    QPointer < CDatabaseHandler > m_database;

public:
    enum EDbTableSettingsColumn
    {
        TableSettingsColumn_parameter = 0x00,
        TableSettingsColumn_humanName,
        TableSettingsColumn_value,
        TableSettingsColumn_type,
        TableSettingsColumn_module,
        TableSettingsColumn_description,
        TableSettingsColumn_private,
        TableSettingsColumn_subgroupName,
        TableSettingsColumn_subgroupHumanName,
    };

/*! функционал для потокобезопасного использования методов */
private slots:
    QList<CSettingsEntry> getSettingsTotalListSlot( const QString &module,
                                                    bool showPrivate );

    CSettingsEntry getSettingSlot(  const QString &name,
                                    const QString &module,
                                    bool showPrivate );

    bool setSettingSlot(            const CSettingsEntry &setting,
                                    bool override );

    bool setSettingsListSlot(const QList < CSettingsEntry > &settingsList ,
                                    bool override );
signals:
    void getSettingsTotalListResult( QList < CSettingsEntry > &/*, QPrivateSignal &*/);
    void getSettingResult( CSettingsEntry &/*, QPrivateSignal &*/);
    void setSettingResult( bool/*, QPrivateSignal &*/);
    void setSettingsListResult( bool/*, QPrivateSignal &*/);

public slots:
    void threadStartedSlot();
    void threadStopedSlot();

/*! Интерфейсы */
public slots:
    bool setSetting(                            const CSettingsEntry &setting,
                                                bool override = true );

    bool setSettingsList(                       const QList < CSettingsEntry > &settingsList,
                                                bool override = true);

    virtual QList < CSettingsEntry > getAllSettings( bool showPrivate,
                                                     const QString &module);

    CSettingsEntry getSetting(                  const QString &name,
                                                bool showPrivate = true,
                                                /* TODO Need to fix*/
                                                const QString module = QCoreApplication::applicationName() );

/*! Метод получения указателя на singleton объекта */
public:
    CSettingsDatabaseProvider( QObject * parent = nullptr );
    virtual ~CSettingsDatabaseProvider()
    {

    }
    const CDatabaseHandler * getDatabaseTransport();
// Костыли
    static const char *getTableName();
    static const char *getTableColumn(CSettingsDatabaseProvider::EDbTableSettingsColumn );
};

#endif // CSETTINGSDATABASEPROVIDER_H
