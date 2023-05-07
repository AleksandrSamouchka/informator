#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QtCore/QSettings>
#include <QtCore/QMutex>

#include <memory>

enum class CSettingsErrors_t : uint8_t
{
    noError,       // порядок, нет ошибки
    fileOpenError, // Не удалось открыть файл настроек
    fieldReadError, // Не удалось найти нужного поля в файле настроек
    fieldHasWrongValueError // Полю настроек присвоено некорректное значение
};

/** ***********************************************************************
 * @class CSettings
 * @brief Класс одиночка (singleton). Доступ к файлу с настройками.
 * В файле global.h должен быть определен параметр SETT::SETTINGS_FILE_NAME - имя файла с настройками
 * Для чтения или записи параметра необходимо указать группу и имя параметра, например
 * readValue( "group_1/" + "param_1", 0 ) либо сразу readValue( "group_1/param_1" )
 * Желательно группы и имена параметров поместить в константы. Например
 * static const QString SETT_GROUP1 = "group_1/";
 * static const QString SETT_GR1_PARAM1 = "param_1";
 * readValue( SETT_GROUP1 + SETT_GR1_PARAM1, 0 )
 ** ***********************************************************************/
class CSettings : public QSettings
{
    Q_OBJECT

protected:
    CSettings(QObject *parent = nullptr);
    bool settingsFileErr;
    static QString path;    // Путь к файлу настроек
    static std::unique_ptr<CSettings> settings;
    static QMutex instanceMutex;
public:
    virtual ~CSettings() {}
    static QVariant readValue(const QString &key, const QVariant &defaultValue = QVariant());
    static void writeValue(const QString &key, const QVariant &value);
    // Не хватило отзывчивости двух предыдущих методов, добавлены еще два, чтобы получать
    // сообщения об ошибках
    static QVariant readValue(const QString &key, CSettingsErrors_t &err, const QVariant &defaultValue = QVariant());
    static void writeValue(const QString &key, CSettingsErrors_t &err, const QVariant &value);
    static QString fileName();
};

#endif // CSETTINGS_H
