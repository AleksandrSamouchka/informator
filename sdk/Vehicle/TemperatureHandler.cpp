#include "TemperatureHandler.h"

#include "CSettingsHandler/CSettingsHandler.h"
#include "CSettingsHandler/CSettingsEntry/CSettingsVariantExtender.h"
#include "Mqtt/Topics/Settings/CSettingsSettingChangeTopic.h"
#include "Mqtt/Topics/Stm/Ro/CStmRoOneWireTopic.h"
#include "Mqtt/Topics/Liaz/CLiazSalonTempTopic.h"
#include "Mqtt/Topics/Liaz/CLiazAmbientTempTopic.h"
#include "CSettingsHandler/CSettingsHandler.h"
#include "CSettingsHandler/CSettingsEntry/CSettingsVariantExtender.h"

namespace
{
    const QString ONE_WIRE = "1-wire";
}

void TemperatureHandler::init(QObject *app)
{
    getInstance().m_app = app;

    getInstance().subscribeToTopics();

    getInstance().readTempSensorsSettings();
}

void TemperatureHandler::onSettingsChange()
{
    // TODO: maybe it dont need to realize. But if someone change sensor source in setting
    // he will need to reload BK
}

void TemperatureHandler::onRoOneWireMsg()
{
    auto topic = static_cast<CStmRoOneWireTopic*>(sender());

    if (topic == nullptr)
    {
        qCritical() << Q_FUNC_INFO << "Incorrect sender()";
        return;
    }
    if (!topic->initFromData(topic->data()))
    {
        qCritical() << "Incorrect data CStmRoOneWireTopic";
        return;
    }
    if (!topic->isValid())
    {
        qCritical() << "Invalid CStmRoOneWireTopic";
        return;
    }

    QList < double > tempValues{};
    for (const auto & i : topic->getSensors())
    {
        if (i.m_id == m_termOutsideName && m_tempSourceOutside == ETempSource::oneWire)
        {
            m_tempOutside = qRound(i.m_value);
        }
        if (m_termInsideNames.contains( i.m_id ) && m_tempSourceInside == ETempSource::oneWire)
        {
            tempValues.append(i.m_value);
        }
    }
    if (m_tempSourceInside == ETempSource::oneWire)
    {
        if (tempValues.isEmpty())
        {
            m_tempInside = 0;
        }
        else
        {
            double summTemp{ 0.0 };
            for (const auto & i : tempValues)
            {
                summTemp += i;
            }
            m_tempInside = qRound(summTemp / tempValues.size());
        }
    }
    if (m_tempSourceOutside == ETempSource::oneWire || m_tempSourceInside == ETempSource::oneWire)
    {
        emit temperatureChanged();
    }
}

void TemperatureHandler::onCanSalonTempTopic()
{
    auto topic = static_cast<CLiazSalonTempTopic*>(sender());

    if (topic == nullptr)
    {
        qCritical() << Q_FUNC_INFO << "Incorrect sender()";
        return;
    }
    if (!topic->initFromData(topic->data()))
    {
        qCritical() << "Incorrect data CLiazSalonTempTopic";
        return;
    }
    if (!topic->isValid())
    {
        qCritical() << "Invalid CLiazSalonTempTopic";
        return;
    }
    if (m_tempSourceInside != ETempSource::can) return;
    if (qRound(topic->getValue()) == m_tempInside) return;

    m_tempInside = qRound(topic->getValue());
    emit temperatureChanged();
}

void TemperatureHandler::onCanAmbientTempTopic()
{
    auto topic = static_cast<CLiazAmbientTempTopic*>(sender());

    if (topic == nullptr)
    {
        qCritical() << Q_FUNC_INFO << "Incorrect sender()";
        return;
    }
    if (!topic->initFromData(topic->data()))
    {
        qCritical() << "Incorrect data CLiazSalonTempTopic";
        return;
    }
    if (!topic->isValid())
    {
        qCritical() << "Invalid CLiazSalonTempTopic";
        return;
    }
    if (m_tempSourceOutside != ETempSource::can) return;
    if (qRound(topic->getValue()) == m_tempOutside) return;

    m_tempOutside = qRound(topic->getValue());
    emit temperatureChanged();
}

void TemperatureHandler::subscribeToTopics()
{
    // Subscribe for setting change
    auto settingsChangeTopic = new CSettingsSettingChangeTopic();
    settingsChangeTopic->setParent(&getInstance());
    QMetaObject::invokeMethod(getInstance().m_app, "onSubscribeToTopic", Qt::QueuedConnection,
                              Q_ARG(CMqttTopic*, settingsChangeTopic));
    connect(settingsChangeTopic, &CSettingsSettingChangeTopic::messageReceived,
            &getInstance(), &TemperatureHandler::onSettingsChange);

    // Subscribe for one wire temperature topic
    auto roOneWireTopic = new CStmRoOneWireTopic();
    roOneWireTopic->setParent(&getInstance());
    QMetaObject::invokeMethod(getInstance().m_app, "onSubscribeToTopic", Qt::QueuedConnection,
                              Q_ARG(CMqttTopic*, roOneWireTopic));
    connect(roOneWireTopic, &CStmRoOneWireTopic::messageReceived, &getInstance(),
            &TemperatureHandler::onRoOneWireMsg);

    // Subscribe for CAN temperature topic
    auto canSalonTempTopic = new CLiazSalonTempTopic(2);
    auto canAmbientTempTopic = new CLiazAmbientTempTopic();

    canSalonTempTopic->setParent(&getInstance());
    canAmbientTempTopic->setParent(&getInstance());

    QMetaObject::invokeMethod(getInstance().m_app, "onSubscribeToTopic", Qt::QueuedConnection,
                              Q_ARG(CMqttTopic*, canSalonTempTopic));
    QMetaObject::invokeMethod(getInstance().m_app, "onSubscribeToTopic", Qt::QueuedConnection,
                              Q_ARG(CMqttTopic*, canAmbientTempTopic));

    connect(canSalonTempTopic, &CLiazSalonTempTopic::messageReceived,
            &getInstance(), &TemperatureHandler::onCanSalonTempTopic);
    connect(canAmbientTempTopic, &CLiazAmbientTempTopic::messageReceived,
            &getInstance(), &TemperatureHandler::onCanAmbientTempTopic);
}

void TemperatureHandler::readTempSensorsSettings()
{
    CSettingsVariantExtender setting(
        CSettingsHandler::getInstance()->
        getSetting( CSettingsHandler::SETT_TEMP_1WIRE_OUTSIDE_NAME,
                   true,
                   CSettingsHandler::SETT_TEMP_GROUP ) );
    if ( setting.isValid() && setting.getValueType() == CSettingsVariantExtender::ESupportTypes::Type_string )
    {
        m_termOutsideName = setting.toString();
    }


    setting = CSettingsVariantExtender(
        CSettingsHandler::getInstance()->
        getSetting( CSettingsHandler::SETT_TEMP_1WIRE_SALON_NAME,
                   true,
                   CSettingsHandler::SETT_TEMP_GROUP ) );
    if ( setting.isValid() && setting.getValueType() == CSettingsVariantExtender::ESupportTypes::Type_string )
    {
        QString data = setting.toString();
        data.replace( " ,", "," );
        data.replace( ", ", "," );
        m_termInsideNames = data.split( "," );
    }


    setting = CSettingsVariantExtender(
        CSettingsHandler::getInstance()->getSetting( CSettingsHandler::SETT_TEMP_SOURCE_SALON,
                                                    true,
                                                    CSettingsHandler::SETT_TEMP_GROUP ));
    if ( setting.isValid() && setting.getValueType() == CSettingsVariantExtender::ESupportTypes::Type_string )
    {
        m_tempSourceInside = setting.toString() == ONE_WIRE ? ETempSource::oneWire : ETempSource::can;
        qInfo() << Q_FUNC_INFO << "Inside temperature source is" << setting.toString();
    }


    setting = CSettingsVariantExtender(
        CSettingsHandler::getInstance()->getSetting( CSettingsHandler::SETT_TEMP_SOURCE_OUTSIDE,
                                                    true,
                                                    CSettingsHandler::SETT_TEMP_GROUP ));
    if ( setting.isValid() && setting.getValueType() == CSettingsVariantExtender::ESupportTypes::Type_string )
    {
        m_tempSourceOutside = setting.toString() == ONE_WIRE ? ETempSource::oneWire : ETempSource::can;
        qInfo() << Q_FUNC_INFO << "Outside temperature source is" << setting.toString();
    }
}

TemperatureHandler::TemperatureHandler(QObject *parent) : QObject(parent)
{}
