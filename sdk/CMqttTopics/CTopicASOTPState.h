#ifndef SDK_CTOPICASOTPSTATE_H
#define SDK_CTOPICASOTPSTATE_H

#include <CMqttTopics/CTopicValueEntry.h>


class CTopicASOTPState : public CTopicValueEntry {
public:
    int m_remote{0};    //связь с выносным пультом (1 – связь есть, 0 – связи нет)
    int m_mode{0};      //режим работы системы пожаротушения (1 – режим «Автомат», 0 – режим «Ручной»)
    int m_in1{0};       //состояние пожарного извещателя 1-го канала (0x00 – отключено, 0x01 – норма, 0x02 – обрыв линии связи, 0x03 – зафиксирован пожар, 0x04 – короткое замыкание)
    int m_in2{0};       //состояние пожарного извещателя 2-го канала
    int m_in3{0};       //состояние пожарного извещателя 3-го канала
    int m_in4{0};       //состояние пожарного извещателя 4-го канала
    int m_out1{0};      //состояние средств пожаротушения 1-го канала (0x00 – отключено, 0x01 – норма, 0x02 – обрыв линии связи, 0x03 – произведен запуск средства пожаротушения)
    int m_out2{0};      //состояние средств пожаротушения 2-го канала
    int m_out3{0};      //состояние средств пожаротушения 3-го канала
    int m_out4{0};      //состояние средств пожаротушения 4-го канала
    int m_btn1{0};      //состояние кнопки аварийного пуска (1 – кнопка нажата; 0 – кнопка не нажата)
    int m_btn2{0};      //состояние кнопки ручного пуска (1 – кнопка нажата; 0 – кнопка не нажата)

public:
    explicit CTopicASOTPState(const QByteArray &json);
    CTopicASOTPState();
    CTopicASOTPState(const CTopicASOTPState &other) = default;

    bool initFromData(const CTopicValueEntry &value) override;
    QByteArray prepareData() const override;
    ~CTopicASOTPState() override = default;

    bool operator==(const CTopicASOTPState &right) const;
    bool operator!=(const CTopicASOTPState &right) const;
};


#endif //SDK_CTOPICASOTPSTATE_H
