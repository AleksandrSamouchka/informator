#ifndef DOORMACHINE_H
#define DOORMACHINE_H

// Qt
#include <QObject>
#include <QVector>

// SDK
#include <Mqtt/Topics/Bus/CBusDoorTopic.h>
#include <Vehicle/CBusType.h>

#include <bitset>

enum EDoor : int {
    Door1 = 0,
    Door2 = 1,
    Door3 = 2,
    Door4 = 3,
    MaxDoorCount
};


class DoorMachine : public QObject
{
Q_OBJECT
public:
    enum EVehicleState {
        VehicleClosed,  // Все двери закрыты          - ТС открыто
        VehicleOpen     // Открыта хотя бы одна дверь - ТС закрыто
    };

    enum EDoorState : int {
        Error  = -1,  // Двери нет
        Closed =  0,  // Закрыта
        Open   =  1   // Открыта
    };

    explicit DoorMachine(QObject* parent = nullptr);
    virtual ~DoorMachine() = default;

    /* У app должен быть реализован слот onSubscribeToTopic(CMqttTopic*), подписывающися на топик в аргументе.
       Вызывать init нужно после того как MqttClient подключился к локальному брокеру, например в методе onAttach() AppController'а */
    void init(QObject* app);

    EDoorState doorState(EDoor idx) const;
    QString doorStateString(EDoor idx) const;

signals:
    void vehicleStateChanged(EVehicleState);
    void doorStatesChanged(QVector<EDoorState>);
    void doorStateChanged(EDoor,EDoorState);

private slots:
    void onSettingsChanged();
    void onDoorState();

protected:
    virtual void setDoorCount(int count);
    int m_doorCount{EDoor::MaxDoorCount};
    QVector<CBusDoorTopic*> m_doorTopics{};
    QVector<EDoorState> m_doorStates{};

    EVehicleState m_vehicleState{EVehicleState::VehicleClosed};
};

#endif //DOORMACHINE_H
