#ifndef ROUTENETWORKSTRUCTURES_H
#define ROUTENETWORKSTRUCTURES_H

// Qt
#include <QVector>
#include <QString>
#include <QStringList>


enum WhenPlayResources
{
    Beforestation,
    Afterstation,
    Onstation
};

struct Text_t
{    
    //! время экспозиции
    double exposure;    
    //! Список строк для отображения
    QStringList text;

    Text_t()
        : exposure(-1)
    {}

    bool operator==(const Text_t& other) const
    {
        if (exposure != other.exposure) return false;
        if (text != other.text) return false;
        return true;
    }

    bool operator !=(const Text_t& other) const
    {
        return !operator==(other);
    }

    bool empty() const
    {
        return (exposure < 0);
    }
};

//! Настройки для LED-панелей
struct LedSettings_t
{
    QString frontPanelHAlign;
    QString sidePanelHAlign;
    QString salonPanelHAlign;

    QString routeNumberColorText;
    QString routeNumberColorBackGround;
    QString textNumberColorText;
    QString textNumberColorBackGround;
};

struct TextRC_t
{
    //! описатель первой строки Led-панели
    Text_t line1;
    //! описатель второй  строки Led-панели
    Text_t line2;

    bool operator==(const TextRC_t& other) const
    {
        if (line1 != other.line1) return false;
        if (line2 != other.line2) return false;
        return true;
    }

    bool operator !=(const TextRC_t& other) const
    {
        return !operator==(other);
    }

    // Второй линии не модет быть без первой. Если нет первой линии => текстовой ресурс пуст
    bool empty()
    {
        return line1.empty();
    }
};

struct Audio_t
{
    //! идентификатор
    int id;
    //! номер в последовательности воспроизведения
    int sorting;
    //! пауза ПЕРЕД воспроизведением
    double pause;
    //! наименование файла
    QString name;
    //! октетная строка sha256 описываемого файла
    QString crc;        
    //! Размер файла в байтах
    int fileSize;
    //! ссылка для скачивания ресурса
    QString url;

    QString realFileName() const
    {
        return (crc + ".mp3");
    }

    bool operator==(const Audio_t& other) const
    {
        if (id != other.id) return false;
        if (sorting != other.sorting) return false;
        if (pause != other.pause) return false;        
        if (name != other.name) return false;
        if (crc != other.crc) return false;        
        if (fileSize != other.fileSize) return false;        
        if (url != other.url) return false;
        return true;
    }

    bool operator !=(const Audio_t& other) const
    {
        return !operator==(other);
    }
};

struct AudioRCStation_t
{
    //! массив объектов-описателей audio ресурсов типа Audio_t применяемых при подъезде к остановке
    QVector<Audio_t> stopIn;
    //! массив объектов-описателей audio ресурсов типа Audio применяемых при отъезде от остановки
    QVector<Audio_t> stopOut;
    //! массив объектов-описателей audio ресурсов типа Audio применяемых непосредственно на остановке
    QVector<Audio_t> stopOver;

    bool wasPlayedBeforestation;
    bool wasPlayedAfterStation;
    bool wasPlayedOnStation;

    AudioRCStation_t ()
        : wasPlayedBeforestation(false)
        , wasPlayedAfterStation(false)
        , wasPlayedOnStation(false)
    {}


    bool operator==(const AudioRCStation_t& other) const
    {
        if (stopIn != other.stopIn) return false;
        if (stopOut != other.stopOut) return false;
        if (stopOver != other.stopOver) return false;
        if (wasPlayedBeforestation != other.wasPlayedBeforestation) return false;
        if (wasPlayedAfterStation != other.wasPlayedAfterStation) return false;
        if (wasPlayedOnStation != other.wasPlayedOnStation) return false;

        return true;
    }

    bool operator !=(const AudioRCStation_t& other) const
    {
        return !operator==(other);
    }
};

struct AudioRC_t
{
    QVector<Audio_t> audioData;

    bool operator==(const AudioRC_t& other) const
    {
        if (audioData != other.audioData) return false;
        return true;
    }

    bool operator !=(const AudioRC_t& other) const
    {
        return !operator==(other);
    }
};

struct TextResource_t
{
    TextRC_t ledNum;
    //! описатель текстовых ресурсов для боковой LED панели типа - пустой для остановок и маршрута, не пустой для рейсов
    TextRC_t ledSide;
    //! описатель текстовых ресурсов для передней LED панели типа - пустой для остановок и маршрута, не пустой для рейсов
    TextRC_t ledFront;
    //! описатель текстовых ресурсов для салонной LED
    TextRC_t ledInternal;
    //! описатель текстовых ресурсов для передней LED
    TextRC_t lcdText;

    bool operator==(const TextResource_t& other) const
    {
        if (ledNum != other.ledNum) return false;
        if (ledSide != other.ledSide) return false;
        if (ledFront != other.ledFront) return false;
        if (ledInternal != other.ledInternal) return false;
        if (lcdText != other.lcdText) return false;

        return true;
    }

    bool operator !=(const TextResource_t& other) const
    {
        return !operator==(other);
    }
};

struct Resources_t
{
    //! описатель текстовых ресурсов
    TextResource_t text;
    //! описатель audio ресурсов
    AudioRC_t audio;
    //! Аудиоданные для внешних громкоговорителей
    AudioRC_t externalSpeaker;
    bool operator==(const Resources_t& other) const
    {
        if (text.ledNum != other.text.ledNum) return false;
        if (text.ledSide != other.text.ledSide) return false;
        if (text.ledFront != other.text.ledFront) return false;
        if (text.ledInternal != other.text.ledInternal) return false;
        if (text.lcdText != other.text.lcdText) return false;
        if (audio != other.audio) return false;
        if (externalSpeaker != other.externalSpeaker) return  false;
        return true;
    }

    bool operator !=(const Resources_t& other) const
    {
        return !operator==(other);
    }
};

struct ATextResources
{
    TextResource_t stopIn;
    TextResource_t stopOut;
    TextResource_t stopOver;

    bool operator==(const ATextResources& other) const
    {
        if (stopIn != other.stopIn) return false;
        if (stopOut != other.stopOut) return false;
        if (stopOver != other.stopOver) return false;
        return true;
    }

    bool operator !=(const ATextResources& other) const
    {
        return !operator==(other);
    }
};

struct AResources_t
{
    AudioRCStation_t audio;
    ATextResources text;

    bool operator==(const AResources_t& other) const
    {
        if (audio != other.audio) return false;
        if (text != other.text) return false;
        return true;
    }

    bool operator !=(const AResources_t& other) const
    {
        return !operator==(other);
    }
};

#endif // ROUTENETWORKSTRUCTURES_H
