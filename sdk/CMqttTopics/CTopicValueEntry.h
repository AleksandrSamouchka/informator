#ifndef CTOPICVALUEENTRY_H
#define CTOPICVALUEENTRY_H

/** ***********************************************************************
 * @class CTopicValueEntry
 * @brief Базовый класс для сообщений из топиков
 * @author Мальнев А
 * @date 22-01-2019
 ** ***********************************************************************/

#include <CConsoleLogger/CConsoleLogger.h>

#include <QtCore/QVariant>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>

/** ***********************************************************************
 * @class CTopicValueEntry
 * @brief класс, умеющий работать с настройками в Mqtt топиках
 ** ***********************************************************************/

#define GET_TOPIC_INDEX(index)  CTopicValueEntry::ETopicsNamesIndex::index
#define GET_TOPIC_NAME(index) CTopicValueEntry::getTopicName( CTopicValueEntry::ETopicsNamesIndex::index )
//#define GET_TOPIC_NAME_PTR(index) CTopicValueEntry::getTopicNamePtr( CTopicValueEntry::ETopicsNamesIndex::index )


class CTopicValueEntry
{
public:
/*! Список индексов топиков, заодно и тип данных сообщения */
    enum class ETopicsNamesIndex : int32_t
    {
    /* navigation */
        Topic_Navigation_gnssState = 0,     /*!< + */

    /* connection */
        Topic_Connection_connectionState,   /*!< + */
        Topic_Connection_voiceDialCmd,      /*!< + */
        Topic_Connection_voiceDialState,    /*!< + */

    /* informator */
        Topic_Informator_selectedRoute,     /*!< + */
//#if 0
//        Topic_Informator_selectedSubroute,  /*!< + */
        Topic_Informator_selectedMode,      /*!< + */
        Topic_Informator_cmdToChangeMode,   /*!< + */
//#endif
        Topic_Informator_routeState,        /*!< + */
        Topic_Informator_routesList,        /*!< + */
//#if 0
        Topic_Informator_playStop,          /*!< + */
        Topic_Informator_playDoors,         /*!< + */
//#endif

    /* recorder */
        Topic_Recorder_cameras,             /*!< + */
        Topic_Recorder_records,             /*!< + */
        Topic_Recorder_upload,              /*!< + */
        Topic_Recorder_uploadStatus,        /*!< + */

    /* logger */
        Topic_Logger_guiMessage,            /*!< + */

    /* settings */
        Topic_Settings_settingChangeCmd,    /*!< + */

    /* stm */
        Topic_StmRwLte_power,               /*!< + */
        Topic_StmRwLte_simNumber,           /*!< + */
        Topic_StmRwGnssPower,               /*!< + */
        Topic_StmRwGnss_backup,             /*!< + */
        Topic_StmRw_tangdest,               /*!< + */
        Topic_StmRw_out2,                   /*!< + */

        Topic_StmRo_ign,                    /*!< + */
        Topic_StmRo_button,                 /*!< + */
        Topic_StmRo_tamper,                 /*!< + */
        Topic_StmRo_boardVoltage,           /*!< + */
        Topic_StmRo_batVoltage,             /*!< + */
        Topic_StmRo_mfi,                    /*!< + */
        Topic_StmRo_oneWire,                /*!< + */
        Topic_StmRo_fwWersion,

        Topic_StmIcf_CAN1TX,
        Topic_StmIcf_CAN1RX,
        Topic_StmIcf_CAN2TX,
        Topic_StmIcf_CAN2RX,
        Topic_StmIcf_CAN3TX,
        Topic_StmIcf_CAN3RX,

        Topic_StmIcf_RS4851TX,
        Topic_StmIcf_RS4851RX,
        Topic_StmIcf_RS4852TX,
        Topic_StmIcf_RS4852RX,
        Topic_StmIcf_RS4853TX,
        Topic_StmIcf_RS4853RX,

        Topic_StmIfc_RS2321TX,
        Topic_StmIfc_RS2321RX,
        Topic_StmIfc_RS2322TX,
        Topic_StmIfc_RS2322RX,
        Topic_StmIfc_RS2323TX,
        Topic_StmIfc_RS2323RX,

    /* gui */
        Topic_Gui_Sos,                      /*!< + */

    /* liaz */
        Topic_Liaz_door1,
        Topic_Liaz_door2,
        Topic_Liaz_door3,
        Topic_Liaz_door4,
        Topic_Liaz_sosButton,
        Topic_Liaz_autoInformer,
        Topic_Liaz_transmissionR,
        Topic_Liaz_transmissionF,
        Topic_Liaz_vehState,
        Topic_Liaz_voltage,
        Topic_Liaz_ambientTemp,
        Topic_Liaz_insideTemp,
        Topic_Liaz_tahoSpeed,
        Topic_Liaz_oilTemp,
        Topic_Liaz_coolantTemp,
        Topic_Liaz_saloonTemp1,
        Topic_Liaz_saloonTemp2,
        Topic_Liaz_saloonTemp3,
        Topic_Liaz_saloonTemp4,
        Topic_Liaz_motorTemp,
        Topic_Liaz_fuelLevelPrimary,
        Topic_Liaz_fuelLevelSecondary,

        Topic_Liaz_kneeling,
        Topic_Liaz_apparel,
        Topic_Liaz_disabledPersonReq,
        Topic_Liaz_routePointer,
        Topic_Liaz_webasto,
        Topic_Liaz_lightLevel,
        Topic_Liaz_passangerReq1,
        Topic_Liaz_passangerReq2,
        Topic_Liaz_passangerReq3,
        Topic_Liaz_passangerReq4,
        Topic_Liaz_door1Opening,
        Topic_Liaz_door2Opening,
        Topic_Liaz_door3Opening,
        Topic_Liaz_door4Opening,
        Topic_Liaz_door1Closing,
        Topic_Liaz_door2Closing,
        Topic_Liaz_door3Closing,
        Topic_Liaz_door4Closing,
        Topic_Liaz_errors,

    /* audio */
        Topic_Audio_muteSalon,
        Topic_Audio_muteDriver,
        Topic_Audio_muteSpeakingCity,
        Topic_Audio_muteFreeChannel,
        Topic_Audio_salonVolume,
        Topic_Audio_driverVolume,
        Topic_Audio_speakingCityVolume,
        Topic_Audio_freeChannelVolume,
        Topic_Audio_micVolume,
        Topic_Audio_trackTo,
        Topic_Audio_trackToStatus,
/*        Topic_Audio_trackToSalon,
        Topic_Audio_trackToDriver,
        Topic_Audio_trackToSpeakingCity,
        Topic_Audio_trackToSalonStatus,
        Topic_Audio_trackToDriverStatus,
        Topic_Audio_trackToSpeakingCityStatus,
*/

    /* tracker */
        Topic_Tracker_ServerIps,
        Topic_Tracker_FormalizeMessageReceived,
        Topic_Tracker_FormalizeMessageNeedSend,

    /* integration */
        Topic_Integration_Validators,

    /* watchdog */
        Topic_Watchdog_can,
        Topic_Watchdog_gnss,
        Topic_Watchdog_gui,
        Topic_Watchdog_informator,
        Topic_Watchdog_logger,
        Topic_Watchdog_networker,
        Topic_Watchdog_recorder,
        Topic_Watchdog_stm,
        Topic_Watchdog_tracker,
        Topic_Watchdog_lte,
        Topic_Watchdog_liaz,
        Topic_Watchdog_mmm,
        Topic_Watchdog_led,
        Topic_Watchdog_led2,
        Topic_Watchdog_ii2,
        Topic_Watchdog_onvif,
        Topic_Watchdog_integrationMqtt,
        Topic_Watchdog_sysmon,
        Topic_Watchdog_configMgr,
        Topic_Watchdog_cvs,

    /* diagnostic */
        Topic_Diagnostic_can,
        Topic_Diagnostic_gnss,
        Topic_Diagnostic_gui,
        Topic_Diagnostic_informator,
        Topic_Diagnostic_logger,
        Topic_Diagnostic_networker,
        Topic_Diagnostic_recorder,
        Topic_Diagnostic_stm,
        Topic_Diagnostic_tracker,
        Topic_Diagnostic_lte,
        Topic_Diagnostic_liaz,
        Topic_Diagnostic_mmm,
        Topic_Diagnostic_led,
        Topic_Diagnostic_led2,
        Topic_Diagnostic_ii2,
        Topic_Diagnostic_onvif,

    /* onvif */
        Topic_Onvif_foundCameras,
    /* system */
        Topic_System_HealthHddStatus,
        Topic_System_HealthHddPrepare,
        Topic_System_runasroot,

    /* validators */
        Topic_Validators_state,
        Topic_Validators_passangerCounter,
        Topic_IncarnetInspector_state,

    /* integration */
        Topic_Integration_IsReady,
        Topic_Integration_Informator,
        Topic_Integration_ValidatorState,
        Topic_Integration_IncarnetInspectorState,
        Topic_Integration_PassengerCounter,
        Topic_Integration_GnssState,
        Topic_Integration_Device_Alarm,
        Topic_Integration_Device_Ignition,
        Topic_Integration_FuelLevelPrimary,
        Topic_Integration_FuelLevelSecondary,
        Topic_Integration_AmbientTemp,
        Topic_Integration_InsideTemp,
        Topic_Integration_SaloonTemp1,
        Topic_Integration_SaloonTemp2,
        Topic_Integration_SaloonTemp3,
        Topic_Integration_SaloonTemp4,

        /*  */
        Topic_unknown,
        /*  */
        
        Topic_Last_Item
    };

private:
    QByteArray m_data = QByteArray{};
    QString m_topic = QString{};
    ETopicsNamesIndex m_topicIndex{ ETopicsNamesIndex::Topic_unknown };
    bool m_valid = false;
    bool m_retain{ false };

protected:
    bool setValidState( bool valid );

    void printEmptyJsonMessage() const;
    void printUnknownFieldsListMessage(const QStringList &filedsList) const;

public:
    CTopicValueEntry() = default;
    CTopicValueEntry( const CTopicValueEntry & other ) = default;
//    explicit CTopicValueEntry( const QByteArray json, const QString topicName );
    explicit CTopicValueEntry( const QByteArray &json, const ETopicsNamesIndex topicIndex );
    explicit CTopicValueEntry( const QByteArray &json, const QString &topicName );
    virtual ~CTopicValueEntry() = default;
    virtual bool initFromData( const CTopicValueEntry &value );        /*!< Используется при чтении данных из топика */
    virtual QByteArray prepareData() const;   /*!< Используется для записи данных в топик */

    QByteArray getData() const;
    QString getTopic() const;
    ETopicsNamesIndex getTopicIndex() const;
    bool isValid() const;
    bool isRetain() const;

    void setRetain( bool retain );

/** ***********************************************************************
 * @fn getTopicName
 * @brief [static] Возвращает наименование топика
 * @param const ETopicsNamesIndex topicIndex
 * @return QString
 ** ***********************************************************************/
    static QString getTopicName( const ETopicsNamesIndex topicIndex );
/** ***********************************************************************
 * @fn getTopicNameIndex
 * @brief [static] Возвращает индекс топика по его имени
 * @param const QString & topic
 * @return ETopicsNamesIndex
 ** ***********************************************************************/
    static ETopicsNamesIndex getTopicNameIndex( const QString &topic );
};


typedef CTopicValueEntry::ETopicsNamesIndex TopicIndex;


#endif // CTOPICVALUEENTRY_H
