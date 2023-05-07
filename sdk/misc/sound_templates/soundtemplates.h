#ifndef SOUNDTEMPLATES_H
#define SOUNDTEMPLATES_H

#include <QObject>
#include <QString>
#include <QVector>
#include <misc/informator/informator_defines.h>


// Типы для аудиошаблонов проигрывания файлов
enum class TemplateStepType_t
{
    audioFile,
    standardWord,
    pause,
    unknown
};


struct TemplateStep_t
{
    TemplateStepType_t type;
    QString audioFilepath;
    QString stWordName;
    int pauseMs;
    TemplateStep_t():
            type(TemplateStepType_t::unknown),
            pauseMs(0)
    {}
    bool operator==(const TemplateStep_t &other) const
    {
        if(type!= other.type) return false;
        if(audioFilepath!= other.audioFilepath) return false;
        if(stWordName!= other.stWordName) return false;
        if(pauseMs != other.pauseMs) return false;
        return true;
    }
    bool operator!=(const TemplateStep_t &other) const
    {
        return !operator==(other);
    }
};


struct StandardWord_t
{
    QString stWordName;
    QString audioFilePath;

    bool operator==(const StandardWord_t &other) const
    {
        if(stWordName!= other.stWordName) return false;
        if(audioFilePath!= other.audioFilePath) return false;
        return true;
    }
    bool operator!=(const StandardWord_t &other) const
    {
        return !operator==(other);
    }
};


struct SoundTemplate_t
{
    uint64_t uId;       // Uid шаблона
    QString templateName;
    QVector<TemplateStep_t> steps;
    QVector<StandardWord_t> stWords;
    SoundTemplate_t():
            uId(0)
    {}
    bool operator==(const SoundTemplate_t &other) const
    {
        if(uId!= other.uId) return false;
        if(templateName!= other.templateName) return false;
        if(steps!= other.steps) return false;
        if(stWords != other.stWords) return false;
        return true;
    }
    bool operator!=(const SoundTemplate_t &other) const
    {
        return !operator==(other);
    }
};


/** ***********************************************************************
 * @class CWatchDog
 * @brief класс шаблонов воспроизведения фразы
 ** ***********************************************************************/
class SoundTemplates: public QObject
{
    Q_OBJECT
public:

    // Публичные методы
    SoundTemplates(QObject *parent = nullptr);
    // bool loadTemplates(const QString &fileName);
    // bool saveTemplates(const QString &fileName);
    void clearTemplates();

    QVector<SoundTemplate_t> templates() const {return mTemplates;}
    //QVector <StandardWord_t> standardWords() const {return mStWords;}

    void setTemplates(const QVector<SoundTemplate_t> &templates) {mTemplates = templates;}
    //void setStandardWords(const QVector<StandardWord_t> &stWords) {mStWords = stWords;}
    //QList<QString> getFilesForPlaying(const SoundTemplate_t te   mpl, тут состояние маршрута еще);


    static QJsonArray soundTemplatesToJsonArray(const QVector<SoundTemplate_t> &templates);
    static QVector<SoundTemplate_t> jsonArrayToSoundTemplates(const QJsonArray &templates, bool &err);
private:
    QVector <SoundTemplate_t> mTemplates;

    static QString templateStepTypeToString(TemplateStepType_t type);
    static TemplateStepType_t stringToStepType(const QString &value);
    //QVector<StandardWord_t> jsonToStandardWords(const QByteArray &json, bool &err);
};

#endif // SOUNDTEMPLATES_H
