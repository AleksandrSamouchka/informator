#include "CSettings.h"
#include <CConsoleLogger/CConsoleLogger.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include <QDebug>

std::unique_ptr<CSettings> CSettings::settings = Q_NULLPTR;
QMutex CSettings::instanceMutex{};

/** ***********************************************************************
 * @fn CSettings [constructor]
 * @brief Конструктор (недоступен извне). Создает файл настроек
 * @param QString - ключ для поиска
 * @param QVariant - значение по умолчанию (если параметр не найден)
 * @return QVariant
 ** ***********************************************************************/
CSettings::CSettings(QObject *parent) :
    QSettings( QDir::current().absoluteFilePath(QString("%1.conf").arg(QCoreApplication::applicationName())),
               Format::IniFormat,
               parent )
{
     if( ! QFile::exists( QSettings::fileName() ) )
     {
         settingsFileErr = true;
     }
     else
     {
         settingsFileErr = false;
     }
}

/** ***********************************************************************
 * @fn readValue [static]
 * @brief метод для чтения параметра из файла настроек
 * @param QString - ключ для поиска
 * @param QVariant - значение по умолчанию (если параметр не найден)
 * @return QVariant
 ** ***********************************************************************/
QVariant CSettings::readValue(const QString &key, const QVariant &defaultValue)
{
    CSettingsErrors_t error{};
    const auto result{ CSettings::readValue( key, error, defaultValue ) };
    switch ( error ) {
    case CSettingsErrors_t::fileOpenError:
        if ( CConsoleLogger::isVerbose() )
        {
            PRINT_CONSOLE_MESSAGE_INFO( QString( "Can't open settings file: %1, using default value: %2 for key: %3" )
                                        .arg(static_cast<QSettings*>(settings.get())->fileName())
                                        .arg(defaultValue.toString())
                                        .arg(key) );
        }
        break;
    case CSettingsErrors_t::fieldReadError:
        if ( CConsoleLogger::isVerbose() )
        {
            PRINT_CONSOLE_MESSAGE_INFO( QString( "Can't find setting %1 within file %2, using default value: %3" )
                                        .arg(key)
                                        .arg(static_cast<QSettings*>(settings.get())->fileName() )
                                        .arg(defaultValue.toString()));
        }
        break;
    case CSettingsErrors_t::fieldHasWrongValueError: // это значение
        break;                                      // используется в клиентском коде
                                                    // На эту метку программа не должна попасть.
    case CSettingsErrors_t::noError:
        break;
    }

    return result;
}

/** ***********************************************************************
 * @fn writeValue [static]
 * @brief метод для записи параметра в файл настроек
 * @param QString - ключ для записи
 * @param QVariant - значение
 * @return none
 ** ***********************************************************************/
void CSettings::writeValue(const QString &key, const QVariant &value)
{
    if ( ! settings )
    {
        QMutexLocker locker( & instanceMutex );
        if ( ! settings )
        {
            settings.reset(new CSettings());
        }
    }
    settings->setValue(key, value);
    settings->sync();
}

/** ***********************************************************************
 * @fn readValue [static]
 * @brief перегруженный метод для чтения параметра из файла настроек
 * @param QString - ключ для поиска
 * @param CSettingsErrors_t& - в эту переменную вернется ошибка выполнения
 * @param QVariant - значение по умолчанию (если параметр не найден)
 * @return QVariant
 ** ***********************************************************************/
QVariant CSettings::readValue(const QString &key, CSettingsErrors_t &err, const QVariant &defaultValue)
{
    if ( ! settings )
    {
        QMutexLocker locker( & instanceMutex );
        if ( ! settings )
        {
            settings.reset(new CSettings());
        }
    }
    if(settings->settingsFileErr) {
        err = CSettingsErrors_t::fileOpenError;
        return defaultValue;
    }
    settings->sync();
    const QVariant errValue(QString("errorValue"));
    QVariant result = settings->value(key, errValue);
    if(result == errValue) {
        err = CSettingsErrors_t::fieldReadError;
        return defaultValue;
    }
    err = CSettingsErrors_t::noError;
    return result;
}

/** ***********************************************************************
 * @fn writeValue [static]
 * @brief перегруженный метод для записи параметра в файл настроек
 * @param QString - ключ для записи
 * @param CSettingsErrors_t& - в эту переменную вернется ошибка выполнения
 * @param QVariant - значение
 * @return none
 ** ***********************************************************************/
void CSettings::writeValue(const QString &key, CSettingsErrors_t &err, const QVariant &value)
{
    if ( ! settings )
    {
        QMutexLocker locker( & instanceMutex );
        if ( ! settings )
        {
            settings.reset(new CSettings());
        }
    }
    if(settings->settingsFileErr) {
        err = CSettingsErrors_t::fileOpenError;
        return;
    }
    err = CSettingsErrors_t::noError;
    settings->setValue(key, value);
    settings->sync();
}

QString CSettings::fileName()
{
    if (settings == nullptr)
    {
        QMutexLocker locker( & instanceMutex );
        if (settings == nullptr)
        {
            settings.reset(new CSettings());
        }
    }
    return static_cast<QSettings*>(settings.get())->fileName();
}
