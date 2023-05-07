#ifndef SDK_CERRORMESSAGEPARSER_H
#define SDK_CERRORMESSAGEPARSER_H

#include <cstdint>
#include <QString>
#include <QHash>

struct ErrorMessage {
//    Методы получения строковых значений для отображения кода ошибки
    QString SA() const { return QString::number(hex, 16).right(2);}
    QString PGN() const { return QString::number(QString::number(hex, 16).mid(2, 4).toUInt(nullptr, 16));}
    QString SPN() const { return (spn == 0) ? QString("-") : QString::number(spn);}
    QString FMI() const {
        if (fmi.isEmpty())
            return QString("-");
        else if(fmi.count() == 1)
            return QString::number(fmi.first());
        else {
            QStringList fmiStrList;
            for(const auto it: fmi){
                fmiStrList << QString::number(it);
            }
            return QString("[%2]").arg(fmiStrList.join(","));
        }
    }

    // общие свойства
    uint16_t    id;
    uint32_t    hex;
    QString     group;
    QString     desc;
    QString     advice;
    bool        dm1_type; // true - тип сообщения DM1

    // DM1
    uint32_t         spn;
    QList<int8_t>   fmi;
    QString         fmi_desc;

    // не DM1
    uint        starting_bit;
    uint        len;
};

class CErrorMessageParser {
public:
    CErrorMessageParser() = default;
    ~CErrorMessageParser() = default;

    bool initFromFile(const QString& fileName);
    ErrorMessage getMessage(const int id) const;

//    QHash<int, QString> m_groups;
    QList<ErrorMessage> m_messages;
};


#endif //SDK_CERRORMESSAGEPARSER_H
