#ifndef CABSTRACTSETTINGSPROVIDER_H
#define CABSTRACTSETTINGSPROVIDER_H

#include <QtCore/QObject>
#include <CSettingsHandler/CSettingsEntry/CSettingsEntry.h>

class CAbstractSettingsProvider : public QObject
{
    Q_OBJECT

signals:

public slots:
    virtual bool setSetting(                            const CSettingsEntry &setting,
                                                        bool override = true ) = 0;

    virtual bool setSettingsList(                       const QList < CSettingsEntry > &settingsList,
                                                        bool override = true ) = 0;
/*
    virtual QList < CSettingsEntry > getAllSettings()
    {
        QString module = QCoreApplication::applicationName();
        return getAllSettings(true, module);
    }

    virtual QList < CSettingsEntry > getAllSettings(bool showPrivate)
    {
        QString module = QCoreApplication::applicationName();
        return getAllSettings(showPrivate, module);
    }
*/
    virtual QList < CSettingsEntry > getAllSettings(    bool showPrivate/* = true*/,
                                                        const QString &module/* = QCoreApplication::applicationName() */) = 0;

    virtual CSettingsEntry getSetting(                  const QString &name,
                                                        bool showPrivate = true,
                                                        /* TODO Need to fix */
                                                        const QString module = QCoreApplication::applicationName() ) = 0;

public:
    CAbstractSettingsProvider( QObject *parent = nullptr );
};

#endif // CABSTRACTSETTINGSPROVIDER_H
