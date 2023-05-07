#include "VehicleSpec.h"

#include "CSettingsHandler/CSettingsHandler.h"
#include "CSettingsHandler/CSettingsEntry/CSettingsVariantExtender.h"
#include "Mqtt/Topics/Settings/CSettingsSettingChangeTopic.h"

#include "Mqtt/Topics/Liaz/CLiazTahospeedTopic.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>


namespace  {

const QString handleUrlFromDB(const QString &url)
{
    if (url.isEmpty()) return QString("");

    QJsonObject obj = QJsonDocument::fromJson(url.toUtf8()).object();

    if (obj["value"].isNull()) return QString("");

    QJsonObject urlObj = obj["value"].toObject();

    return urlObj["cameraUrl"].toString();
}

const QString PRIMARY_URL = "RecorderCameraPrimaryUrl";
const QString SECONDARY_URL = "RecorderCameraSecondaryUrl";


void printSQLError(QSqlQuery &query)
{
    QString errStr = QString( "Error with sql query '%1' : %2" )
            .arg(query.lastQuery(),
                 query.lastError().text());
    PRINT_CONSOLE_MESSAGE_WARN( errStr);
}

}


void VehicleSpec::init(QObject *app)
{
    instance().m_app = app;

    instance().m_vehicleType = readVehicleType();

    const auto& busNumber = readBusNumber();
    if (busNumber != -1)
    {
        qInfo() << Q_FUNC_INFO << "Bus number is: " << busNumber;
        instance().m_busNumber = busNumber;
    }

    auto settingsChangedTopic = new CSettingsSettingChangeTopic();
    settingsChangedTopic->setParent(&instance());
    QMetaObject::invokeMethod(instance().m_app, "onSubscribeToTopic", Qt::QueuedConnection, Q_ARG(CMqttTopic*, settingsChangedTopic));
    connect(settingsChangedTopic, &CSettingsSettingChangeTopic::messageReceived, &instance(), &VehicleSpec::onSettingsChanged);

    instance().m_dbHandler = new CDatabaseHandler(app->metaObject()->className(), &instance());
    instance().m_dbHandler.data()->threadStartedSlot();
}

void VehicleSpec::resetSpeedTopic()
{
    if(!m_speedTopic || m_speedSrc != speedSrc())
    {
        delete m_speedTopic;

        if(m_vehicleType == EBusType::ukvz71_628M)
            m_speedTopic = new CSimpleTopic<double>("NAVIGATION/GNSS_STATE", "Speed");
        else
            m_speedTopic = new CLiazTahospeedTopic();

        m_speedTopic->setParent(this);

        if(m_app){
            QMetaObject::invokeMethod(m_app, "onSubscribeToTopic", Qt::QueuedConnection, Q_ARG(CMqttTopic*, m_speedTopic));
        }
        else {
            qWarning() << "VehicleSpec не инициализирован. Приложение не подписано на топик скорости.";
        }

        m_speedSrc = speedSrc();
        emit speedSrcChanged();
    }
}

void VehicleSpec::onSettingsChanged()
{
    auto* topic = static_cast<CSettingsSettingChangeTopic*>(sender());
    if(topic == nullptr){
        qCritical() << "Некорректный sender!";
        return;
    }
    if (!topic->initFromData(topic->data()))
    {
        qCritical() << "Некорректное содержимое топика CSettingsSettingChangeTopic";
        return;
    }

    if (topic->isValid() &&
            topic->module() == CSettingsHandler::SETT_TRANSPORT_GROUP &&
            topic->settingName() == CSettingsHandler::SETT_TRANSPORT_BUS_TYPE)
    {
        m_vehicleType = readVehicleType();
        emit vehicleTypeChanged();

        resetSpeedTopic();
    }

}

EBusType VehicleSpec::readVehicleType()
{
    // Читаем из тип автобуса
    const CSettingsVariantExtender setting(
                CSettingsHandler::getInstance()->getSetting(
                    CSettingsHandler::SETT_TRANSPORT_BUS_TYPE,
                    true,
                    CSettingsHandler::SETT_TRANSPORT_GROUP ) );
    if ( setting.isValid()
         && setting.getValueType() == CSettingsVariantExtender::ESupportTypes::Type_double )
    {
        return static_cast<EBusType>(setting.toInt());
    }
    else {
        qCritical() << "Ошибка чтения типа транспортного средства из базы данных.";
        return EBusType::unknown;
    }
}

int VehicleSpec::readBusNumber()
{
    const auto& bus_id  = CSettingsHandler::getInstance()->getInstance()->getSetting("externalMqttUniqId", true, "mntconti_integration_mqtt");
    const auto strBusID = CSettingsVariantExtender(bus_id).toString();
    bool isConverted{false};
    strBusID.split("_").last().toInt(&isConverted);
    if (isConverted) return strBusID.split("_").last().toInt();
    qWarning() << Q_FUNC_INFO << "Invalid bus number";
    return -1;
}

CSimpleTopic<double>* VehicleSpec::getSpeedTopic()
{
    if(!instance().m_speedTopic){
        instance().resetSpeedTopic();
    }

    return instance().m_speedTopic;
}

QList<CamDesc> VehicleSpec::getCamList()
{
    QMap<QString, CamDesc> resultMap;

    const char* getTitleAndCameraNumberQuery = "select value, subgroup_human_name, subgroup_name from "
"mntconti_settings where parameter like \'%RecorderCameraName%\';";

    QSqlQuery query = instance().m_dbHandler.data()->getQuery();

    if (query.prepare(getTitleAndCameraNumberQuery))
    {
        if (query.exec())
        {
            while (query.next())
            {
                CamDesc cDesc;
                cDesc.index = query.value(1).toString().split(" ").last().toInt();

                QJsonObject obj = QJsonDocument::fromJson(query.value(0).toString().toUtf8()).object();
                cDesc.humanName = obj["value"].isNull() ? "" : obj["value"].toString();

                resultMap.insert(query.value(2).toString(), cDesc);
            }
        }
        else  printSQLError(query);
    }
    else  printSQLError(query);




    const QString getCameraTitlesQuery =    "SELECT parameter, subgroup_name, value FROM mntconti_settings WHERE "
                                            "parameter LIKE \"%RecorderCameraPrimary%\" "
                                            "OR parameter LIKE \"%RecorderCameraSecondary%\";";

    if (query.prepare(getCameraTitlesQuery))
    {
        if (query.exec())
        {
            while(query.next())
            {
                QString parameter = query.value(0).toString();
                QString cameraName = query.value(1).toString();

                QPair<QString, QString> streams;

                if (parameter.contains(PRIMARY_URL))
                    resultMap[cameraName].primaryUrl = handleUrlFromDB(query.value(2).toString());

                if (parameter.contains(SECONDARY_URL))
                    resultMap[cameraName].secondaryUrl = handleUrlFromDB(query.value(2).toString());
            }
        }
        else
        {
            printSQLError(query);
        }
    }

    printSQLError(query);

//    for(auto it: resultMap.keys()){
//        if(resultMap.value(it).primaryUrl.isEmpty() && resultMap.value(it).secondaryUrl.isEmpty())
//            resultMap.remove(it);
//    }

    return resultMap.values();
}

