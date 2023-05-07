#include "CTopicSystemHealthHddStatusVolumePart.h"

static const char *FIELD_PARTITION = "partition";
static const char *FIELD_ISMOUNTED = "is_mounted";
static const char *FIELD_MOUNTPOINT = "mount_point";
static const char *FIELD_TOTALVOLUME = "total_space";
static const char *FIELD_FREEVOLUME = "free_space";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicSystemHealthHddStatusVolumePart::CTopicSystemHealthHddStatusVolumePart( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_unknown )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicSystemHealthHddStatusVolumePart::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.contains( FIELD_PARTITION ) &&
                 jsonMainObj.contains( FIELD_ISMOUNTED ) &&
                 jsonMainObj.contains( FIELD_MOUNTPOINT ) )
            {
                m_partition = jsonMainObj.value( FIELD_PARTITION ).toString();
                m_isMounted = jsonMainObj.value( FIELD_ISMOUNTED ).toInt();
                m_mountPoint = jsonMainObj.value( FIELD_MOUNTPOINT ).toString();
                if ( jsonMainObj.contains( FIELD_TOTALVOLUME ) )
                {
                    m_totalVolume = jsonMainObj.value( FIELD_TOTALVOLUME ).toString();
                }
                if ( jsonMainObj.contains( FIELD_FREEVOLUME ) )
                {
                    m_freeVolume = jsonMainObj.value( FIELD_FREEVOLUME ).toString();
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
