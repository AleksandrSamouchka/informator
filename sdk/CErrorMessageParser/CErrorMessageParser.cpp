#include "CErrorMessageParser.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

bool CErrorMessageParser::initFromFile(const QString &fileName) {
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Невозможно открыть файл:" << fileName;
        return false;
    }

    QByteArray json = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();

//    auto groups = obj.value("groups").toArray();
    auto alarm_codes = obj.value("alarm_codes").toArray();

//    m_groups.clear();
    m_messages.clear();

//    for(const auto& it: groups){
//        auto group = it.toObject();
//        m_groups[group.value("index").toInt()] = group.value("name").toString();
//    }

    for(const auto& it: alarm_codes){
        auto acode = it.toObject();

        ErrorMessage msg{};
        msg.id = acode.value("id").toInt();
        msg.dm1_type = acode.value("dm1_type").toBool(false);

        bool ok;
        msg.hex = acode.value("hex").toString().toUInt(&ok, 16);
        msg.group = acode.value("group").toString();
        msg.desc = acode.value("description").toString();
        msg.advice = acode.value("advice").toString();

        if(msg.dm1_type){
            msg.spn = acode.value("spn").toInt();
            auto fmi_arr = acode.value("fmi").toArray();
            for(const auto& it: fmi_arr){
                msg.fmi << it.toInt();
            }
            msg.fmi_desc = acode.value("fmi_description").toString();
        }
        else{
            msg.starting_bit = acode.value("starting_bit").toInt();
            msg.len = acode.value("len").toInt();
        }
        m_messages << msg;
    }
    return true;
}

ErrorMessage CErrorMessageParser::getMessage(const int id) const {
    for(const auto& it: m_messages){
        if(it.id == id)
            return it;
    }
    qDebug() << "Сообщение с id =" << id << "не найдено";
    return ErrorMessage();
}
