// Local
#include "DoorMachine.h"

// SDK
#include <CSettingsHandler/CSettingsHandler.h>
#include <CSettingsHandler/CSettingsEntry/CSettingsVariantExtender.h>
#include <Mqtt/Topics/Settings/CSettingsSettingChangeTopic.h>

namespace
{
    constexpr uint8_t FIRST_BIT_DOOR_PRESENT = 0;
    constexpr uint8_t LAST_BIT_DOOR_PRESENT = 3;
    constexpr uint8_t FIRST_BIT_DOOR_STATE = 4;
    constexpr uint8_t LAST_BIT_DOOR_STATE = 7;
}

DoorMachine::DoorMachine(QObject *parent) :
    QObject(parent)
{}

void DoorMachine::init(QObject *app)
{
    // Читаем из тип автобуса, что бы понять количество дверей на ТС
    const CSettingsVariantExtender setting(
                CSettingsHandler::getInstance()->getSetting(
                    CSettingsHandler::SETT_TRANSPORT_BUS_TYPE,
                    true,
                    CSettingsHandler::SETT_TRANSPORT_GROUP ) );
    if ( setting.isValid()
         && setting.getValueType() == CSettingsVariantExtender::ESupportTypes::Type_double )
    {
        setDoorCount(CBusType::doorNumber(static_cast<EBusType>(setting.toInt())));
        PRINT_CONSOLE_MESSAGE_INFO(QStringLiteral("Количество дверей: %1").arg(m_doorCount));
    }

    // Подключаемся к топикам дверей с соответствии с типу ТС
    m_doorTopics.resize(EDoor::MaxDoorCount);
    m_doorStates.resize(EDoor::MaxDoorCount);
    for (auto i = 0; i < EDoor::MaxDoorCount; i++)
    {
        m_doorTopics[i] = new CBusDoorTopic(i + 1);
        connect(m_doorTopics.at(i), &CBusDoorTopic::messageReceived, this, &DoorMachine::onDoorState, Qt::QueuedConnection);
        QMetaObject::invokeMethod(app, "onSubscribeToTopic", Qt::QueuedConnection, Q_ARG(CMqttTopic*, m_doorTopics.at(i)));
        m_doorStates[i] = EDoorState::Closed;
    }

    auto settingsChangedTopic = new CSettingsSettingChangeTopic();
    settingsChangedTopic->setParent(this);
    QMetaObject::invokeMethod(app, "onSubscribeToTopic", Qt::QueuedConnection, Q_ARG(CMqttTopic*, settingsChangedTopic));
    connect(settingsChangedTopic, &CSettingsSettingChangeTopic::messageReceived, this, &DoorMachine::onSettingsChanged);
}

void DoorMachine::onDoorState()
{
    CBusDoorTopic* topic = static_cast<CBusDoorTopic*>(sender());
    if(topic == nullptr)
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Некорректный sender!" );
        return;
    }
    if (!topic->initFromData(topic->data()))
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Некорректное содержимое топика CBusDoor" );
        return;
    }

    // номер двери
    int index = topic->getNum();
    if (index == 0 || index > EDoor::MaxDoorCount)
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Некорректный номер двери в топике" );
        return;
    }
    if (index > m_doorCount)
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Номер двери больше, чем всего дверей у транспорного средства. Проверьте в настройках тип транспоортного средства." );
        return;
    }
    PRINT_CONSOLE_MESSAGE_INFO(QString("Door%1State: %2").arg(index).arg(topic->getValue()));

/*! @brief Логика события все двери закрыты */

    // были ли все двери закрыты, до того как пришло новое состояние двери
    const bool isAllClosed{ ! m_doorStates.contains(EDoorState::Open)};


    const auto newDoorState = topic->getValue() ? EDoorState::Open : EDoorState::Closed;
    const bool changed{m_doorStates[index-1] != newDoorState};

    m_doorStates[index-1] = newDoorState;

    if (changed)
    {
        // закрыты ли все двери с учетом нового события от двери
        bool isNewAllClosed{ ! m_doorStates.contains(EDoorState::Open) };

        if ( isAllClosed ) {
            if (!isNewAllClosed) {
                // одна из дверей открылась, до этого все были закрыты
                m_vehicleState = EVehicleState::VehicleOpen;
                PRINT_CONSOLE_MESSAGE_INFO( "Двери открылись" );
                emit vehicleStateChanged(m_vehicleState);
            }
        }
        else {
            if (isNewAllClosed) {
                // все двери закрылись, до этого хотябы одна была открыта
                // начало движения
                m_vehicleState = EVehicleState::VehicleClosed;
                PRINT_CONSOLE_MESSAGE_INFO("Двери закрылись");
                emit vehicleStateChanged(m_vehicleState);
            }
        }
        emit doorStateChanged(static_cast<EDoor>(index-1), newDoorState);
        emit doorStatesChanged(m_doorStates);
    }
}

void DoorMachine::setDoorCount(int count)
{
    qInfo() << Q_FUNC_INFO << "Set door count to " << count;
    m_doorCount = count;
}

DoorMachine::EDoorState DoorMachine::doorState(EDoor idx) const
{
    if(idx < m_doorStates.count())
        return m_doorStates.at(idx);
    return EDoorState::Error;
}

QString DoorMachine::doorStateString(EDoor idx) const
{
    switch (doorState(idx)) {
    case Open: return "Open";
    case Closed: return "Closed";
    default: return "Error";
    }
}


void DoorMachine::onSettingsChanged()
{
    auto* topic = static_cast<CSettingsSettingChangeTopic*>(sender());
    if(topic == nullptr)
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Некорректный sender!" );
        return;
    }
    if (!topic->initFromData(topic->data()))
    {
        PRINT_CONSOLE_MESSAGE_ERROR( "Некорректное содержимое топика CSettingsSettingChangeTopic" );
        return;
    }

    if (topic->isValid())
    {
        if (topic->module() == CSettingsHandler::SETT_TRANSPORT_GROUP)
        {
            if (topic->settingName() == CSettingsHandler::SETT_TRANSPORT_BUS_TYPE)
            {
                const CSettingsVariantExtender setting(
                            CSettingsHandler::getInstance()->getSetting(
                                CSettingsHandler::SETT_TRANSPORT_BUS_TYPE,
                                true,
                                CSettingsHandler::SETT_TRANSPORT_GROUP ) );
                if (setting.isValid()
                        && setting.getValueType() == CSettingsVariantExtender::ESupportTypes::Type_double)
                {
                    auto busType = static_cast< EBusType >( qRound( setting.toDouble() ) );
                    setDoorCount(CBusType::doorNumber(busType));
                }
            }
        }
    }
}


