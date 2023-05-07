#pragma once

#include <QObject>
#include <QPointer>
#include <CDatabaseHandler/CDatabaseHandler.h>

#include "CBusType.h"
#include "CamDesc.h"
#include "Mqtt/Topics/CSimpleTopic.h"
#include "boost/optional/optional.hpp"


class VehicleSpec : public QObject
{
    Q_OBJECT
public:
    enum class SpeedSrc {
        Tachograph,
        Gnss,
        None
    };


public:
    static VehicleSpec& instance()
    {
        static VehicleSpec inst;
        return inst;
    }
    VehicleSpec(VehicleSpec const&) = delete;
    VehicleSpec(VehicleSpec&&) = delete;
    VehicleSpec& operator=(VehicleSpec const&) = delete;
    VehicleSpec& operator=(VehicleSpec &&) = delete;

protected:
    VehicleSpec() {}
    ~VehicleSpec() override {}

public:
    /* У app должен быть реализован слот onSubscribeToTopic(CMqttTopic*), подписывающися на топик в аргументе.
       Вызывать init нужно после того как MqttClient подключился к локальному брокеру, например в методе onAttach() AppController'а */
    static void init(QObject* app);

    static EBusType vehicleType()
    {
        if(instance().m_vehicleType == EBusType::unknown){
            instance().m_vehicleType = readVehicleType();
        }
        return instance().m_vehicleType;
    }

    static QString vehicleTypeGenitive()
    {
        if (instance().vehicleType() == EBusType::ukvz71_628M)
            return "трамвая";
        else
            return "автобуса";
    }

    static QString vehicleTypeNominative()
    {
        if (instance().vehicleType() == EBusType::ukvz71_628M)
            return "трамвай";
        else
            return "автобус";
    }

    static boost::optional<uint64_t> busNumber() {return instance().m_busNumber;}

    static int doorCount() {return CBusType::doorNumber(static_cast<EBusType>(instance().m_vehicleType));}

    static CSimpleTopic<double> *getSpeedTopic();

    static QList<CamDesc> getCamList();

private:
    SpeedSrc speedSrc() const {return m_vehicleType == EBusType::ukvz71_628M ? SpeedSrc::Gnss : SpeedSrc::Tachograph;}

signals:
    void vehicleTypeChanged();
    void speedSrcChanged();

protected slots:
    void onSettingsChanged();

protected:
    QObject* m_app;
    EBusType m_vehicleType{EBusType::unknown};
    boost::optional<uint64_t> m_busNumber;
    static EBusType readVehicleType();
    static int readBusNumber();

    CSimpleTopic<double>* m_speedTopic;
    SpeedSrc m_speedSrc{SpeedSrc::None};
    void resetSpeedTopic();

    QPointer<CDatabaseHandler> m_dbHandler = nullptr;
};

