#include "CTopicASOTPState.h"

namespace {
    const char * TOPIC = "LIAZ/ASOTPSTATE";

    const char *FIELD_REMOTE = "Remote";
    const char *FIELD_MODE = "Mode";
    const char *FIELD_IN1 = "In1";
    const char *FIELD_IN2 = "In2";
    const char *FIELD_IN3 = "In3";
    const char *FIELD_IN4 = "In4";
    const char *FIELD_OUT1 = "Out1";
    const char *FIELD_OUT2 = "Out2";
    const char *FIELD_OUT3 = "Out3";
    const char *FIELD_OUT4 = "Out4";
    const char *FIELD_BTN1 = "Btn1";
    const char *FIELD_BTN2 = "Btn2";
}

CTopicASOTPState::CTopicASOTPState(const QByteArray &json) :
        CTopicValueEntry(json, TOPIC)
{}

CTopicASOTPState::CTopicASOTPState() :
        CTopicValueEntry(QByteArray{}, TOPIC)
{}

bool CTopicASOTPState::initFromData(const CTopicValueEntry &value) {
    bool res{false};
    const QJsonObject jsonMainObj(QJsonDocument::fromJson(value.getData()).object());
    if (!jsonMainObj.isEmpty()) {
        if (jsonMainObj.contains(FIELD_REMOTE) &&
            jsonMainObj.contains(FIELD_MODE) &&
            jsonMainObj.contains(FIELD_IN1) &&
            jsonMainObj.contains(FIELD_IN2) &&
            jsonMainObj.contains(FIELD_IN3) &&
            jsonMainObj.contains(FIELD_IN4) &&
            jsonMainObj.contains(FIELD_OUT1) &&
            jsonMainObj.contains(FIELD_OUT2) &&
            jsonMainObj.contains(FIELD_OUT3) &&
            jsonMainObj.contains(FIELD_OUT4) &&
            jsonMainObj.contains(FIELD_BTN1) &&
            jsonMainObj.contains(FIELD_BTN2)
                ) {
            m_remote = jsonMainObj.value(FIELD_REMOTE).toInt();
            m_mode = jsonMainObj.value(FIELD_MODE).toInt();
            m_in1 = jsonMainObj.value(FIELD_IN1).toInt();
            m_in2 = jsonMainObj.value(FIELD_IN2).toInt();
            m_in3 = jsonMainObj.value(FIELD_IN3).toInt();
            m_in4 = jsonMainObj.value(FIELD_IN4).toInt();
            m_out1 = jsonMainObj.value(FIELD_OUT1).toInt();
            m_out2 = jsonMainObj.value(FIELD_OUT2).toInt();
            m_out3 = jsonMainObj.value(FIELD_OUT3).toInt();
            m_out4 = jsonMainObj.value(FIELD_OUT4).toInt();
            m_btn1 = jsonMainObj.value(FIELD_BTN1).toInt();
            m_btn2 = jsonMainObj.value(FIELD_BTN2).toInt();
            res = true;
        }
        else{
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else{
        printEmptyJsonMessage();
    }
    setValidState(res);
    return res;
}

QByteArray CTopicASOTPState::prepareData() const {
    QJsonObject jsonMainObject{};

    jsonMainObject.insert(FIELD_REMOTE, m_remote);
    jsonMainObject.insert(FIELD_MODE, m_mode);
    jsonMainObject.insert(FIELD_IN1, m_in1);
    jsonMainObject.insert(FIELD_IN2, m_in2);
    jsonMainObject.insert(FIELD_IN3, m_in3);
    jsonMainObject.insert(FIELD_IN4, m_in4);
    jsonMainObject.insert(FIELD_OUT1, m_out1);
    jsonMainObject.insert(FIELD_OUT2, m_out2);
    jsonMainObject.insert(FIELD_OUT3, m_out3);
    jsonMainObject.insert(FIELD_OUT4, m_out4);
    jsonMainObject.insert(FIELD_BTN1, m_btn1);
    jsonMainObject.insert(FIELD_BTN2, m_btn2);

    return QJsonDocument(jsonMainObject).toJson(QJsonDocument::Compact);
}

bool CTopicASOTPState::operator==(const CTopicASOTPState &right) const {
    if (m_remote == right.m_remote &&
        m_mode == right.m_mode &&
        m_in1 == right.m_in1 &&
        m_in2 == right.m_in2 &&
        m_in3 == right.m_in3 &&
        m_in4 == right.m_in4 &&
        m_out1 == right.m_out1 &&
        m_out2 == right.m_out2 &&
        m_out3 == right.m_out3 &&
        m_out4 == right.m_out4 &&
        m_btn1 == right.m_btn1 &&
        m_btn2 == right.m_btn2
            ) {
        return true;
    }

    return false;
}

bool CTopicASOTPState::operator!=(const CTopicASOTPState &right) const {
    if (m_remote == right.m_remote &&
        m_mode == right.m_mode &&
        m_in1 == right.m_in1 &&
        m_in2 == right.m_in2 &&
        m_in3 == right.m_in3 &&
        m_in4 == right.m_in4 &&
        m_out1 == right.m_out1 &&
        m_out2 == right.m_out2 &&
        m_out3 == right.m_out3 &&
        m_out4 == right.m_out4 &&
        m_btn1 == right.m_btn1 &&
        m_btn2 == right.m_btn2
            ) {
        return false;
    }
    return true;
}
