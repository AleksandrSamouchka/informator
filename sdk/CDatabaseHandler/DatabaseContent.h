#ifndef DATABASECONTENT_H
#define DATABASECONTENT_H

/** ***********************************************************************
 * @name DatabaseContent
 * @brief Константы для работы с БД - таблицы и их столбцы
 * @author Мальнев А
 * @date 08-02-2019
 ** ***********************************************************************/

#include <QtCore/QObject>

/** ***********************************************************************
 * @namespace Db
 * @brief Общее пространство имен для БД
 ** ***********************************************************************/
namespace Db
{
/** ***********************************************************************
 * @namespace TrackerPoints
 * @brief Для таблицы tracker_points
 ** ***********************************************************************/
    namespace TrackerPoints
    {
        const QString TABLE{ "tracker_points" };
        enum EColumns
        {
            TrackerPointColumn_time = 0,
            TrackerPointColumn_lat,
            TrackerPointColumn_lon,
            TrackerPointColumn_alt,
            TrackerPointColumn_speed,
            TrackerPointColumn_direction,
            TrackerPointColumn_sat_count,
            TrackerPointColumn_in_battery,
            TrackerPointColumn_in_sos,
            TrackerPointColumn_in_voice_call,
            TrackerPointColumn_in_move,
            TrackerPointColumn_batt_voltage,
            TrackerPointColumn_odometr,
            TrackerPointColumn_pdop,
            TrackerPointColumn_analog_in_1,
            TrackerPointColumn_analog_in_2,
            TrackerPointColumn_analog_in_3,
            TrackerPointColumn_analog_in_4,
            TrackerPointColumn_counter_in_1,
            TrackerPointColumn_counter_in_2,
            TrackerPointColumn_counter_in_3,
            TrackerPointColumn_counter_in_4,
            TrackerPointColumn_discret_in_1,
            TrackerPointColumn_discret_in_2,
            TrackerPointColumn_discret_in_3,
            TrackerPointColumn_discret_in_4,
            TrackerPointColumn_discret_in_5,
            TrackerPointColumn_discret_in_6,
            TrackerPointColumn_discret_in_7,
            TrackerPointColumn_discret_in_8,
            TrackerPointColumn_discret_out_1,
            TrackerPointColumn_discret_out_2,
            TrackerPointColumn_discret_out_3,
            TrackerPointColumn_discret_out_4,
            TrackerPointColumn_discret_out_5,
            TrackerPointColumn_discret_out_6,
            TrackerPointColumn_discret_out_7,
            TrackerPointColumn_discret_out_8,
            TrackerPointColumn_gprs_state,
            TrackerPointColumn_csq,
/*40*/            
            TrackerPointColumn_server_1_state,
            TrackerPointColumn_server_2_state,
            TrackerPointColumn_server_3_state,
            TrackerPointColumn_server_4_state,
            TrackerPointColumn_fuel_level_0,
            TrackerPointColumn_fuel_level_1,
            TrackerPointColumn_temp_sensor_status_0,
            TrackerPointColumn_temp_sensor_status_1,
            TrackerPointColumn_temp_sensor_status_2,
            TrackerPointColumn_temp_sensor_status_3,
            TrackerPointColumn_temp_sensor_status_4,
            TrackerPointColumn_temp_sensor_status_5,
            TrackerPointColumn_temp_sensor_status_6,
            TrackerPointColumn_temp_sensor_status_7,
            TrackerPointColumn_temp_sensor_status_8,
            TrackerPointColumn_temp_sensor_status_9,
            TrackerPointColumn_temp_sensor_value_0,
            TrackerPointColumn_temp_sensor_value_1,
            TrackerPointColumn_temp_sensor_value_2,
            TrackerPointColumn_temp_sensor_value_3,
            TrackerPointColumn_temp_sensor_value_4,
            TrackerPointColumn_temp_sensor_value_5,
            TrackerPointColumn_temp_sensor_value_6,
            TrackerPointColumn_temp_sensor_value_7,
            TrackerPointColumn_temp_sensor_value_8,
            TrackerPointColumn_temp_sensor_value_9,

            TrackerPointColumn_irma_door_in1,
            TrackerPointColumn_irma_door_in2,
            TrackerPointColumn_irma_door_in3,
            TrackerPointColumn_irma_door_in4,
            TrackerPointColumn_irma_door_out1,
            TrackerPointColumn_irma_door_out2,
            TrackerPointColumn_irma_door_out3,
            TrackerPointColumn_irma_door_out4,
            TrackerPointColumn_irma_present_door,
        };
        const QStringList COLUMNS{
            "time",
            "lat",
            "lon",
            "alt",
            "speed",
            "direction",
            "sat_count",
            "in_battery",
            "in_sos",
            "in_voice_call",
            "in_move",
            "batt_voltage",
            "odometr",
            "pdop",
            "analog_in_1",
            "analog_in_2",
            "analog_in_3",
            "analog_in_4",
            "counter_in_1",
            "counter_in_2",
            "counter_in_3",
            "counter_in_4",
            "discret_in_1",
            "discret_in_2",
            "discret_in_3",
            "discret_in_4",
            "discret_in_5",
            "discret_in_6",
            "discret_in_7",
            "discret_in_8",
            "discret_out_1",
            "discret_out_2",
            "discret_out_3",
            "discret_out_4",
            "discret_out_5",
            "discret_out_6",
            "discret_out_7",
            "discret_out_8",
            "gprs_state",
            "csq",
            "server_1_state",
            "server_2_state",
            "server_3_state",
            "server_4_state",
            "fuel_level_0",
            "fuel_level_1",
            "temp_sensor_status_0",
            "temp_sensor_status_1",
            "temp_sensor_status_2",
            "temp_sensor_status_3",
            "temp_sensor_status_4",
            "temp_sensor_status_5",
            "temp_sensor_status_6",
            "temp_sensor_status_7",
            "temp_sensor_status_8",
            "temp_sensor_status_9",
            "temp_sensor_value_0",
            "temp_sensor_value_1",
            "temp_sensor_value_2",
            "temp_sensor_value_3",
            "temp_sensor_value_4",
            "temp_sensor_value_5",
            "temp_sensor_value_6",
            "temp_sensor_value_7",
            "temp_sensor_value_8",
            "temp_sensor_value_9",
            "irma_door_in1",
            "irma_door_in2",
            "irma_door_in3",
            "irma_door_in4",
            "irma_door_out1",
            "irma_door_out2",
            "irma_door_out3",
            "irma_door_out4",
            "irma_present_door",
        };
        inline QString getColumn( const int column )
        {
            return COLUMNS.value( column, "" );
        }
    }

/** ***********************************************************************
 * @namespace FormalizedMessages
 * @brief Для таблицы formalized_messages
 ** ***********************************************************************/
    namespace FormalizedMessages
    {
        const QString TABLE{ "formalized_messages" };
        enum EColumns
        {
            FormalizedMessagesColumn_id,
            FormalizedMessagesColumn_time,
            FormalizedMessagesColumn_direction,
            FormalizedMessagesColumn_is_processed,
            FormalizedMessagesColumn_is_readed,
            FormalizedMessagesColumn_server_id,
            FormalizedMessagesColumn_request_id,
            FormalizedMessagesColumn_message_id,
            FormalizedMessagesColumn_message,
        };
        const QStringList COLUMNS{
            "id",
            "time",
            "direction",
            "is_processed",
            "is_readed",
            "server_id",
            "request_id",
            "message_id",
            "message",
        };
        inline QString getColumn( const int column )
        {
            return COLUMNS.value( column, "" );
        }
    }


}

#endif // DATABASECONTENT_H
