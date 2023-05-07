#include <CConsoleLogger/CConsoleLogger.h>
#include "CTopicValueEntry.h"

namespace
{
    const QList < QString > TOPIC_NAMES = QList < QString >
    {
        "NAVIGATION/GNSS_STATE",

        "CONNECTION/CONNECTION_STATE",
        "CONNECTION/VOICE_DIAL_CMD",         /*!< cmd */
        "CONNECTION/VOICE_DIAL_STATE",

        "INFORMATOR/SELECTED_ROUTE",         /*!< cmd */
//#if 0
//        "INFORMATOR/SELECTED_SUBROUTE",      /*!< cmd */
        "INFORMATOR/SELECTED_MODE",
        "INFORMATOR/CMD_TO_CHANGE_MODE",     /*!< cmd */
//#endif
        "INFORMATOR/ROUTE_STATE",
        "INFORMATOR/ROUTES_LIST",
//#if 0
        "INFORMATOR/PLAY_STATION",           /*!< cmd */
        "INFORMATOR/PLAY_DOORS",             /*!< cmd */
//#endif

        "RECORDER/CAMERAS",
        "RECORDER/RECORDS",
        "RECORDER/UPLOAD",                   /*!< cmd */
        "RECORDER/UPLOAD_STATUS",

        "LOGGER/GUI_MESSAGE",

        "SETTINGS/SETTING_CHANGE",            /*!< cmd */

        "STM/RW/LTE/POWER",
        "STM/RW/LTE/SIMNUMBER",
        "STM/RW/GNSS/POWER",
        "STM/RW/GNSS/BACKUP",
        "STM/RW/TANGDEST", 
        "STM/RW/OUT2",

        "STM/RO/IGN",
        "STM/RO/BUTTON",
        "STM/RO/TAMPER",
        "STM/RO/BOARDVOLTAGE",
        "STM/RO/BATVOLTAGE",
        "STM/RO/MFI",
        "STM/RO/ONEWIRE",
        "STM/RO/FW_VERSION",

        "STM/IFC/CAN1TX",
        "STM/IFC/CAN1RX",
        "STM/IFC/CAN2TX",
        "STM/IFC/CAN2RX",
        "STM/IFC/CAN3TX",
        "STM/IFC/CAN3RX",

        "STM/IFC/RS4851TX",
        "STM/IFC/RS4851RX",
        "STM/IFC/RS4852TX",
        "STM/IFC/RS4852RX",
        "STM/IFC/RS4853TX",
        "STM/IFC/RS4853RX",

        "STM/IFC/RS2321TX",
        "STM/IFC/RS2321RX",
        "STM/IFC/RS2322TX",
        "STM/IFC/RS2322RX",
        "STM/IFC/RS2323TX",
        "STM/IFC/RS2323RX",

        "GUI/SOS",

        "LIAZ/DOOR1",
        "LIAZ/DOOR2",
        "LIAZ/DOOR3",
        "LIAZ/DOOR4",
        "LIAZ/SOSBUTTON",
        "LIAZ/AUTOINFORMER",
        "LIAZ/TRANSMISSIONR",
        "LIAZ/TRANSMISSIONF",
        "LIAZ/VEHSTATE",
        "LIAZ/VOLTAGE",
        "LIAZ/AMBIENTTEMP",
        "LIAZ/INSIDETEMP",
        "LIAZ/TAHOSPEED",
        "LIAZ/OILTEMP",
        "LIAZ/COOLANTTEMP",
        "LIAZ/SALOONTEMP1",
        "LIAZ/SALOONTEMP2",
        "LIAZ/SALOONTEMP3",
        "LIAZ/SALOONTEMP4",
        "LIAZ/MOTORTEMP",
        "LIAZ/FUEL_LEVEL_PRIMARY",
        "LIAZ/FUEL_LEVEL_SECONDARY",

        "LIAZ/KNEELING",
        "LIAZ/APPAREL",
        "LIAZ/DISABLEDPERSONREQ",
        "LIAZ/ROUTEPOINTER",
        "LIAZ/WEBASTO",
        "LIAZ/LIGHTLEVEL",
        "LIAZ/PASSENGERREQ1",
        "LIAZ/PASSENGERREQ2",
        "LIAZ/PASSENGERREQ3",
        "LIAZ/PASSENGERREQ4",
        "LIAZ/DOOR1OPENING",
        "LIAZ/DOOR2OPENING",
        "LIAZ/DOOR3OPENING",
        "LIAZ/DOOR4OPENING",
        "LIAZ/DOOR1CLOSING",
        "LIAZ/DOOR2CLOSING",
        "LIAZ/DOOR3CLOSING",
        "LIAZ/DOOR4CLOSING",
        "LIAZ/ERRORS",

        "AUDIO/MUTE_SALON",
        "AUDIO/MUTE_DRIVER",
        "AUDIO/MUTE_SPEAKING_CITY",
        "AUDIO/MUTE_FREE_CHANNEL",
        "AUDIO/SALON_VOLUME",
        "AUDIO/DRIVER_VOLUME",
        "AUDIO/SPEAKING_CITY_VOLUME",
        "AUDIO/FREE_CHANNEL_VOLUME",
        "AUDIO/MIC_VOLUME",
        "AUDIO/TRACK_TO",
        "AUDIO/TRACK_TO/STATUS",
/*        "AUDIO/TRACK_TO_SALON",
        "AUDIO/TRACK_TO_DRIVER",
        "AUDIO/TRACK_TO_SPEAKING_CITY",
        "AUDIO/TRACK_TO_SALON/STATUS",
        "AUDIO/TRACK_TO_DRIVER/STATUS",
        "AUDIO/TRACK_TO_SPEAKING_CITY/STATUS",
*/

        "TRACKER/SERVER_IPS",
        "TRACKER/FORMALIZE_MESSAGE_RECEIVED",
        "TRACKER/FORMALIZE_MESSAGE_NEED_SEND",

        "INTEGRATION/VALIDATORS",

        "WATCHDOG/MNTCONTI_CAN_KEEPALIVE",
        "WATCHDOG/MNTCONTI_GNSS_KEEPALIVE",
        "WATCHDOG/MNTCONTI_GUI_KEEPALIVE",
        "WATCHDOG/MNTCONTI_INFORMATOR_KEEPALIVE",
        "WATCHDOG/MNTCONTI_LOGGER_KEEPALIVE",
        "WATCHDOG/MNTCONTI_NETWORKER_KEEPALIVE",
        "WATCHDOG/MNTCONTI_RECORDER_KEEPALIVE",
        "WATCHDOG/MNTCONTI_STM_KEEPALIVE",
        "WATCHDOG/MNTCONTI_TRACKER_KEEPALIVE",
        "WATCHDOG/MNTCONTI_LTE_KEEPALIVE",
        "WATCHDOG/MNTCONTI_LIAZ_KEEPALIVE",
        "WATCHDOG/MNTCONTI_MMM_KEEPALIVE",
        "WATCHDOG/MNTCONTI_LED_KEEPALIVE",
        "WATCHDOG/MNTCONTI_LED2_KEEPALIVE",
        "WATCHDOG/MNTCONTI_II2_KEEPALIVE",
        "WATCHDOG/MNTCONTI_ONVIF_KEEPALIVE",
        "WATCHDOG/MNTCONTI_INTEGRATION_MQTT_KEEPALIVE",
        "WATCHDOG/MNTCONTI_SYSMON_KEEPALIVE",
        "WATCHDOG/MNTCONTI_CONFIG_MGR_KEEPALIVE",
        "WATCHDOG/MNTCONTI_CVS_KEEPALIVE",

        "DIAGNOSTIC/MNTCONTI_CAN",
        "DIAGNOSTIC/MNTCONTI_GNSS",
        "DIAGNOSTIC/MNTCONTI_GUI",
        "DIAGNOSTIC/MNTCONTI_INFORMATOR",
        "DIAGNOSTIC/MNTCONTI_LOGGER",
        "DIAGNOSTIC/MNTCONTI_NETWORKER",
        "DIAGNOSTIC/MNTCONTI_RECORDER",
        "DIAGNOSTIC/MNTCONTI_STM",
        "DIAGNOSTIC/MNTCONTI_TRACKER",
        "DIAGNOSTIC/MNTCONTI_LTE",
        "DIAGNOSTIC/MNTCONTI_LIAZ",
        "DIAGNOSTIC/MNTCONTI_MMM",
        "DIAGNOSTIC/MNTCONTI_LED",
        "DIAGNOSTIC/MNTCONTI_LED2",
        "DIAGNOSTIC/MNTCONTI_II2",
        "DIAGNOSTIC/MNTCONTI_ONVIF",

        "ONVIF/FOUND_CAMERAS",

        "SYSTEM/HEALTH/HDD_STATUS",
        "SYSTEM/HEALTH/HDD_PREPARE",
        "SYSTEM/RUN_AS_ROOT",

        "VALIDATORS/STATE",
        "VALIDATORS/PASSANGER_COUNTER",
        "INCARNET_INSPECTOR/STATE",

        "INTEGRATION/ISREADY",
        "INTEGRATION/INFORMATOR",
        "INTEGRATION/VALIDATORS/STATE",
        "INTEGRATION/INCARNET_INSPECTOR/STATE",
        "INTEGRATION/PASSENGER_COUNTER",
        "INTEGRATION/GNSS_STATE",
        "INTEGRATION/ALARM",
        "INTEGRATION/IGNITION",
        "INTEGRATION/FUEL_LEVEL_PRIMARY",
        "INTEGRATION/FUEL_LEVEL_SECONDARY",
        "INTEGRATION/AMBIENT_TEMP",
        "INTEGRATION/INSIDE_TEMP",
        "INTEGRATION/SALOONTEMP1",
        "INTEGRATION/SALOONTEMP2",
        "INTEGRATION/SALOONTEMP3",
        "INTEGRATION/SALOONTEMP4",

        "UNKNOWN_TOPIC",
        ""
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicValueEntry::CTopicValueEntry( const QByteArray &json, const CTopicValueEntry::ETopicsNamesIndex topicIndex ) :
    m_data( json ),
    m_topic( getTopicName( topicIndex ) ),
    m_topicIndex{ topicIndex },
    m_valid( ! json.isEmpty() )
{ }

CTopicValueEntry::CTopicValueEntry( const QByteArray &json, const QString &topicName ) :
    m_data( json ),
    m_topic( topicName ),
    m_valid( ! json.isEmpty() )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief заготовка для метода инициализации полей из json
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicValueEntry::initFromData(const CTopicValueEntry &value)
{
    Q_UNUSED( value )
    return false;
}

/** ***********************************************************************
 * @fn prepareData
 * @brief заготовка для метода формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicValueEntry::prepareData() const
{
    return QByteArray{};
}

/** ***********************************************************************
 * @fn setValidState
 * @brief протектный метод для установки флага валидности
 * @param bool
 * @return none
 ** ***********************************************************************/
bool CTopicValueEntry::setValidState(bool valid)
{
    m_valid = valid;
    return m_valid;
}

/** ***********************************************************************
 * @brief методы получения значений свойств объекта
 ** ***********************************************************************/
QByteArray CTopicValueEntry::getData() const
{
    return m_data;
}

QString CTopicValueEntry::getTopic() const
{
    return m_topic;
}

CTopicValueEntry::ETopicsNamesIndex CTopicValueEntry::getTopicIndex() const
{
    return m_topicIndex;
}

bool CTopicValueEntry::isValid() const
{
    return m_valid;
}

bool CTopicValueEntry::isRetain() const
{
    return m_retain;
}

void CTopicValueEntry::setRetain(bool retain)
{
    m_retain = retain;
}

/** ***********************************************************************
 * @fn getTopicName      [static]
 * @brief метод для получения имени топика
 * @param ETopicsNamesIndex
 * @return QString
 ** ***********************************************************************/
QString CTopicValueEntry::getTopicName( const CTopicValueEntry::ETopicsNamesIndex topicIndex )
{
    return TOPIC_NAMES.value( static_cast< int >( topicIndex ), "" );
}

/** ***********************************************************************
 * @fn getTopicNameIndex      [static]
 ** ***********************************************************************/
CTopicValueEntry::ETopicsNamesIndex CTopicValueEntry::getTopicNameIndex(const QString &topic)
{
    ETopicsNamesIndex nameIndex{ ETopicsNamesIndex::Topic_unknown };
    const int __index{ TOPIC_NAMES.indexOf( topic ) };
//    PRINT_CONSOLE_MESSAGE_INFO( QString::number( __index ) );
    if ( __index > 0 || __index == 0 )
    {
        nameIndex = static_cast< ETopicsNamesIndex >( __index );
    }
    return nameIndex;
}

void CTopicValueEntry::printEmptyJsonMessage() const
{
    PRINT_CONSOLE_MESSAGE_WARN( "Пустой JSON" );
}

void CTopicValueEntry::printUnknownFieldsListMessage(const QStringList &filedsList) const
{
    PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " + filedsList.join( "," ) );
}
