#ifndef CSETTINGSHANDLER_H
#define CSETTINGSHANDLER_H

#include <CSettingsHandler/CAbstractSettingsProvider.h>
#include <CSettingsHandler/CSettingsDatabaseProvider.h>
#include <QtCore/QPointer>
#include <QtCore/QScopedPointer>
#include <QtCore/QMutex>

/** ***********************************************************************
 * @class CSettingsHandler
 * @brief Класс одиночка (singleton). Обработчик настроек для приложения
 * В данный момент настройки храняться только в БД - используется CSettingsDatabaseProvider
 * Но при желании можно будет легко добавить и другие способы хранения настроек:
 * файл, mqtt и т.п.
 ** ***********************************************************************/
class CSettingsHandler : public CAbstractSettingsProvider
{
    Q_OBJECT
    QPointer < CSettingsDatabaseProvider > m_settingsDatabase{};

public:
/*! @brief Наименования глобальных настроек */
    static const QString SETT_GLOBAL_MODULE;
/*! @brief Настройки температуры */
    static const QString SETT_TEMP_GROUP;
    static const QString SETT_TEMP_SOURCE_SALON;
    static const QString SETT_TEMP_SOURCE_OUTSIDE;
    static const QString SETT_TEMP_1WIRE_SALON_NAME;
    static const QString SETT_TEMP_1WIRE_OUTSIDE_NAME;

    static const QString SETT_TEMP_NOTUSED_SOURCE;
/*! @brief Информация о БК */
    static const QString SETT_INFO_GROUP;
    static const QString SETT_INFO_MANUFDATE;
    static const QString SETT_INFO_SN;
/*! @brief Информация о ТранспортномСредстве */
    static const QString SETT_TRANSPORT_GROUP;
    static const QString SETT_TRANSPORT_DOOR_NUMBER;
    static const QString SETT_TRANSPORT_BUS_TYPE;
/*    enum class EBusType
    {
        nefaz5299 = 1,
        nefaz5299_intern = 2,
        nefaz6282 = 3,
        nefaz6282_intern = 4,
        nefaz5299_new = 5,
        nefaz6282_new = 6,

        liaz621365_79 = 10,
        liaz621365_80_intern = 11,
        liaz6274 = 12,
        liaz6274_intern = 13,

        manLionsCoachR07 = 20,
    };
    static const QHash < int, QString > BUS_TYPE_NAMES;
*/

/*! @brief Настройки БК */
    static const QString SETT_BKSETTINGS_GROUP;
    static const QString SETT_BKSETTINGS_TIMEZONE;

protected:
    CSettingsHandler( QObject * parent = nullptr );
    virtual ~CSettingsHandler() {}
signals:
    void ready( /*QPrivateSignal &*/);

protected slots:
    void threadStartedSlot();
    void threadStopedSlot();

/*! Интерфейсы */
public slots:
    bool setSetting(const CSettingsEntry &setting,
                    bool override = true );

    bool setSettingsList(const QList < CSettingsEntry > &settingsList,
                                                bool override = true);

/* TODO Need to fix */
    inline QList < CSettingsEntry > getAllSettings()
    {
        QString module = QCoreApplication::applicationName();
        return getAllSettings(true, module);
    }
    inline QList < CSettingsEntry > getAllSettings(bool showPrivate)
    {
        QString module = QCoreApplication::applicationName();
        return getAllSettings(showPrivate, module);
    }

    virtual QList < CSettingsEntry > getAllSettings( bool showPrivate, const QString &module/* = QCoreApplication::applicationName()*/ );

    CSettingsEntry getSetting(                  const QString &name,
                                                bool showPrivate = true,
                                                const QString module = QCoreApplication::applicationName() );

/*! Метод получения указателя на singleton объекта */
public:
    static CSettingsHandler * getInstance();
    static void deinitSettingsHandler( );

private:
    static QPointer<CSettingsHandler> handler;
    static QScopedPointer<QThread> handlerThread;
    static QMutex initHandlerMutex;
    static QMutex instanceMutex;
};

CSettingsEntry settings_entry( const QString &name, bool showPrivate = true );

#endif // CSETTINGSHANDLER_H
