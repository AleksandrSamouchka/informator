#include "CTopicValidatorsStatus.h"

static const char *FIELD_ENABLED = "enabled";
static const char *FIELD_INWORK = "in_work";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicValidatorsStatus::CTopicValidatorsStatus( const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Validators_state )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicValidatorsStatus::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.contains( FIELD_ENABLED ) &&
                 jsonMainObj.contains( FIELD_INWORK ) )
            {
                m_enabled = jsonMainObj.value( FIELD_ENABLED ).toInt();
                m_inWork = jsonMainObj.value( FIELD_INWORK ).toInt();
                res = true;
            }
            else
            {
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
QByteArray CTopicValidatorsStatus::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_ENABLED, m_enabled );
    jsonMainObj.insert( FIELD_INWORK, m_inWork );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
