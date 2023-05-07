#ifndef TEXTRCHANDLERS_H
#define TEXTRCHANDLERS_H

// Qt
#include <QStringList>
#include <QJsonObject>
#include <QVariant>
#include <QJsonArray>

// SDK
#include <Mqtt/Topics/Informator/RouteNetworkStructures.h>


namespace TextRcHandlers
{
    inline void fillTextRC(const QJsonObject& obj, TextRC_t& textRc)
    {
        QStringList lines_list;
        lines_list << "line_1" << "line_2";

        foreach (const QString& line, lines_list)
        {
            if (obj.contains(line))
            {
                Text_t text_data;
                QJsonObject line_obj = obj[line].toObject();
                text_data.exposure = line_obj["exposure"].toDouble();
                text_data.text = line_obj["values"].toVariant().toStringList();

                if (line == "line_1")
                {
                    textRc.line1 = text_data;
                }
                else if (line == "line_2")
                {
                    textRc.line2 = text_data;
                }
            }
        }
    }

    inline void textRcToJson(QJsonObject& obj, const TextRC_t& textRc)
    {
        QJsonObject line1_obj;
        QJsonObject line2_obj;

        line1_obj["exposure"] = textRc.line1.exposure;
        line1_obj.insert("values", QJsonArray::fromStringList(textRc.line1.text));

        line2_obj["exposure"] = textRc.line2.exposure;        
        line2_obj.insert("values", QJsonArray::fromStringList(textRc.line2.text));

        obj.insert("line_1", line1_obj);
        obj.insert("line_2", line2_obj);
    }
}

#endif // TEXTRCHANDLERS_H
