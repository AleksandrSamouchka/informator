#include "CTopicSystemHealthHddStatusVolume.h"

static const char *FIELD_CURRTEMP = "curr_temp";
static const char *FIELD_CURRTEMPMIN = "curr_min_temp";
static const char *FIELD_CURRTEMPMAX = "curr_max_temp";
static const char *FIELD_SPECTEMPMIN = "spec_min_temp";
static const char *FIELD_SPECTEMPMAX = "spec_max_temp";
static const char *FIELD_PARTITIONS = "partitions";
static const char *FIELD_SYSTEM = "system";


/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicSystemHealthHddStatusVolume::CTopicSystemHealthHddStatusVolume(const QByteArray &json) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_unknown )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicSystemHealthHddStatusVolume::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.contains( FIELD_CURRTEMP ) &&
                 jsonMainObj.contains( FIELD_CURRTEMPMIN ) &&
                 jsonMainObj.contains( FIELD_CURRTEMPMAX ) &&
                 jsonMainObj.contains( FIELD_SPECTEMPMIN ) &&
                 jsonMainObj.contains( FIELD_SPECTEMPMAX ) &&
                 jsonMainObj.contains( FIELD_PARTITIONS ) )
            {
                m_partition = jsonMainObj.value( FIELD_PARTITIONS ).toString();
                m_currTemp = jsonMainObj.value( FIELD_CURRTEMP ).toInt();
                m_currMinTemp = jsonMainObj.value( FIELD_CURRTEMPMIN ).toInt();
                m_currMaxTemp = jsonMainObj.value( FIELD_CURRTEMPMAX ).toInt();
                m_specMinTemp = jsonMainObj.value( FIELD_SPECTEMPMIN ).toInt();
                m_specMaxTemp = jsonMainObj.value( FIELD_SPECTEMPMAX ).toInt();
                if ( jsonMainObj.contains( FIELD_SYSTEM ) )
                {
                    m_system = jsonMainObj.value( FIELD_SYSTEM ).toInt();
                }
                res = true;
            }
            else
            {
//                PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " +
//                                            jsonMainObj.keys().join( "," ) );
                printUnknownFieldsListMessage(jsonMainObj.keys());
            }
        }
        else
        {
            printEmptyJsonMessage();
        }
        setValidState( res );
    }
    return res;
}
