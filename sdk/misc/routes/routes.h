#ifndef ROUTES_H
#define ROUTES_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QDateTime>
#include <misc/informator/informator_defines.h>
#include <misc/sound_templates/soundtemplates.h>


// Информация о маршруте для хранения ее в файле
struct RouteFullInfo_t
{
    RouteInfo_t title;
    QString defaultLanguage; // Название языка по умолчанию
    uint64_t templatesUid;          // Уникальный номер группы
                                    // аудиошаблонов (половина от md5
                                    // hashsum)
    uint64_t speakingTowntemplatesUid; // Уникальный номер группы
                                    // аудиошаблонов для системы
                                    // "говорящий город"

    //QString routeSoundPath;         // Путь аудиофайла названия маршрута
    //                                // для языка по умолчанию.
    //QString routeEnding1SoundPath;  // [deprecated] Путь аудиофайла названия конечной остановки 1
                                    // маршрута для языка по умолчанию.
    //QString routeEnding2SoundPath;  // [deprecated] Путь аудиофайла названия конечной остановки 2
                                    // маршрута для языка по умолчанию.
    QVector<OtherLanguagesData_t> otherLanguages;   // Данные маршрута
                                                    // для прочих языков

//    LedPanelsSettings ledPanelSettings; // Настройки led-панелей. (для конвертирования из файлов SELENA)

    QVector<SubrouteInfo_t> subroutes; // Перечень подмаршрутов

    RouteFullInfo_t():          // Конструктор без параметров
            templatesUid(0),
            speakingTowntemplatesUid(0)
    {}

    bool operator==(const RouteFullInfo_t &other) const // Реализуем сравнение
    {
        if(title!=other.title) return false;
        if(subroutes!=other.subroutes) return false;
        return true;
    }
    bool operator!=(const RouteFullInfo_t &other) const
    {
        return !operator==(other);
    }
};


#endif // ROUTES_H
