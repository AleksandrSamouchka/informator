#include "CTopicSystemHealthHddPrepare.h"

static const char *FIELD_VOLUME = "volume";
static const char *FIELD_PREPARED = "prepared";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicSystemHealthHddPrepare::CTopicSystemHealthHddPrepare( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_System_HealthHddPrepare )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicSystemHealthHddPrepare::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.contains( FIELD_VOLUME ) &&
                 jsonMainObj.contains( FIELD_PREPARED ) )
            {
                m_volume = jsonMainObj.value( FIELD_VOLUME ).toString();
                m_prepared = jsonMainObj.value( FIELD_PREPARED ).toInt();
                res = true;
            }
            else
            {
//                PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " + jsonMainObj.keys().join( "," ) );
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

/** ***********************************************************************
* @fn prepareData
* @brief метод формирования значения топика из полей структуры
* @param none
* @return QByteArray
** ***********************************************************************/
QByteArray CTopicSystemHealthHddPrepare::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_VOLUME, m_volume );
    jsonMainObj.insert( FIELD_PREPARED, m_prepared );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
