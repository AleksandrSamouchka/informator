#include "soundtemplates.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QFile>


SoundTemplates::SoundTemplates(QObject *parent):
        QObject(parent)
{

}

/*bool SoundTemplates::loadTemplates(const QString &fileName)
{
    Q_UNUSED(fileName);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QByteArray json = file.readAll();
    file.close();
    bool res = true;
    bool err = false;
    mTemplates = jsonToTemplates(json, err);
    if(err == true) res = false;
    mStWords = jsonToStandardWords(json, err);
    if(err == true) res = false;
    return true;
}*/


/*bool SoundTemplates::saveTemplates(const QString &fileName)
{
    Q_UNUSED(fileName);
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QByteArray arr = soundTemplatesToJson(mTemplates, mStWords);
    if(file.write(arr) != arr.size())
    {
        return false;
    }
    file.flush();
    file.close();
    return true;
}*/

void SoundTemplates::clearTemplates()
{
    mTemplates.clear();
}



/*QList<QString> SoundTemplates::getFilesForPlaying(const SoundTemplate_t templ)
{
    for(int i=0; i<templ.steps; i++ )

}*/


QJsonArray SoundTemplates::soundTemplatesToJsonArray(const QVector<SoundTemplate_t> &templates)
{
    //QJsonObject json;
    //QJsonObject mainObj;
    QJsonArray templatesArr;

    for(const SoundTemplate_t &tmpl: templates)
    {
        QJsonObject tmplObj;
        tmplObj.insert("uId", QJsonValue(QString::number(tmpl.uId, 16)));
        tmplObj.insert("templateName", QJsonValue(tmpl.templateName));
        QJsonArray steps;
        for(int i = 0; i<tmpl.steps.size(); i++)
        {
            QJsonObject stepObj;
            const TemplateStep_t &step = tmpl.steps[i];
            stepObj.insert("type", QJsonValue(templateStepTypeToString(step.type)));
            stepObj.insert("audioFilepath", QJsonValue(step.audioFilepath));
            stepObj.insert("stWordName", QJsonValue(step.stWordName));
            stepObj.insert("pauseMs", QJsonValue(step.pauseMs));
            steps.append(QJsonValue(stepObj));
        }
        tmplObj.insert("steps", QJsonValue(steps));

        QJsonArray standardWords;
        for(const StandardWord_t &stWord: tmpl.stWords)
        {
            QJsonObject stWordObj;
            stWordObj.insert("stWordName", QJsonValue(stWord.stWordName));
            stWordObj.insert("audioFilePath", QJsonValue(stWord.audioFilePath));
            standardWords.append(QJsonValue(stWordObj));
        }
        tmplObj.insert("stWords", QJsonValue(standardWords));

        templatesArr.append(QJsonValue(tmplObj));
    }

    //mainObj.insert("standardWords", QJsonValue(standardWords));
    //json.insert("mainObj", QJsonValue(mainObj));
    //QJsonDocument doc(json);
    return templatesArr;
}


QVector<SoundTemplate_t> SoundTemplates::jsonArrayToSoundTemplates(
        const QJsonArray &templates, bool &err)
{
    err = false;
    QVector<SoundTemplate_t> res;
    //QJsonDocument doc = QJsonDocument::fromJson(json);
    //QJsonObject mainObj = doc.object();
    //QJsonObject::iterator mainVal = mainObj.find("templates");
    //if(mainVal == mainObj.end()) {
    //    err = true;
    //    return QVector<Template_t>{};
    //}

    //QJsonArray templates = (*mainVal).toArray();
    res.resize(templates.size());
    for(int i = 0; i<templates.size(); i++) {
        SoundTemplate_t &tmpl = res[i];
        QJsonValue tmplVal = templates[i];
        if(!tmplVal.isObject()) continue;
        QJsonObject tmplObj = tmplVal.toObject();

        QJsonObject::iterator val = tmplObj.find("templateName");
        if(val != tmplObj.end()) {
            tmpl.templateName = (*val).toString();
        }

        // steps
        val = tmplObj.find("steps");
        if(val == tmplObj.end()) {
            continue;
        }
        if(!(*val).isArray())
        {
            continue;
        }

        QJsonArray stepsArr = (*val).toArray();
        tmpl.steps.resize(stepsArr.size());

        for(int j = 0; j<stepsArr.size(); j++)
        {
            TemplateStep_t &step = tmpl.steps[j];
            if(!stepsArr[j].isObject())
            {
                continue;
            }
            QJsonObject stepObj = stepsArr[j].toObject();

            val = stepObj.find("type");
            if(val != stepObj.end())
            {
                 step.type = stringToStepType((*val).toString());
            }
            else step.type = TemplateStepType_t::unknown;

            val = stepObj.find("audioFilepath");
            if(val != stepObj.end())
            {
                 step.audioFilepath = (*val).toString();
            }
            else step.audioFilepath = "";

            val = stepObj.find("stWordName");
            if(val != stepObj.end())
            {
                 step.stWordName = (*val).toString();
            }
            else step.stWordName = "";

            val = stepObj.find("pauseMs");
            if(val != stepObj.end())
            {
                 step.pauseMs = (*val).toInt();
            }
            else step.pauseMs = 0;
        }

        // stWords
        val = tmplObj.find("stWords");
        if(val == tmplObj.end()) {
            continue;
        }
        if(!(*val).isArray())
        {
            continue;
        }

        QJsonArray stWordsArr = (*val).toArray();
        tmpl.steps.resize(stWordsArr.size());

        for(int j = 0; j<stWordsArr.size(); j++)
        {
            TemplateStep_t &step = tmpl.steps[j];
            if(!stWordsArr[j].isObject())
            {
                continue;
            }
            QJsonObject stepObj = stWordsArr[j].toObject();

            val = stepObj.find("audioFilepath");
            if(val != stepObj.end())
            {
                 step.audioFilepath = (*val).toString();
            }
            else step.audioFilepath = "";

            val = stepObj.find("stWordName");
            if(val != stepObj.end())
            {
                 step.stWordName = (*val).toString();
            }
            else step.stWordName = "";
        }
    }
    return res;
}


/*QVector<StandardWord_t> SoundTemplates::jsonArrayToStandardWords(const QByteArray &json, bool &err)
{
    err = false;
    QVector<StandardWord_t> res;
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject mainObj = doc.object();
    QJsonObject::iterator mainVal = mainObj.find("standardWords");
    if(mainVal == mainObj.end()) {
        err = true;
        return QVector<StandardWord_t>{};
    }

    QJsonArray stWords = (*mainVal).toArray();
    res.resize(stWords.size());
    for(int i = 0; i<stWords.size(); i++)
    {
        StandardWord_t &stWord = res[i];
        QJsonValue tmplVal = stWords[i];
        if(!tmplVal.isObject()) continue;
        QJsonObject tmplObj = tmplVal.toObject();

        QJsonObject::iterator val = tmplObj.find("stWordName");
        if(val != tmplObj.end())
        {
            stWord.stWordName = (*val).toString();
        }

        val = tmplObj.find("audioFilePath");
        if(val != tmplObj.end())
        {
            stWord.audioFilePath = (*val).toString();
        }
    }
}*/


QString SoundTemplates::templateStepTypeToString(TemplateStepType_t type)
{
    switch(type)
    {
    case TemplateStepType_t::audioFile:
        return "audioFile";
    case TemplateStepType_t::standardWord:
        return "standardWord";
    case TemplateStepType_t::pause:
        return "pause";
    default:
        return "unknownValue";
    }
}


TemplateStepType_t SoundTemplates::stringToStepType(
        const QString &value)
{
    if(value == "audioFile")
    {
        return TemplateStepType_t::audioFile;
    }
    else if(value == "audioFile")
    {
        return TemplateStepType_t::standardWord;
    }
    else if(value == "audioFile")
    {
        return TemplateStepType_t::pause;
    }
    else
    {
        return TemplateStepType_t::unknown;
    }
}





