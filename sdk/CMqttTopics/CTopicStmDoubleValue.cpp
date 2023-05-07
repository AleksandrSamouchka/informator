#include "CTopicStmDoubleValue.h"

namespace
{
    const char *FIELD_VALUE = "Value";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicStmDoubleValue::CTopicStmDoubleValue( const double value, const QByteArray &json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_StmRo_ign ),
    m_value{ value }
{ }

CTopicStmDoubleValue::CTopicStmDoubleValue()
{

}

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicStmDoubleValue::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_VALUE ) )
        {
            m_value = jsonMainObj.value( FIELD_VALUE ).toDouble();
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
    return res;
}

/** ***********************************************************************
* @fn prepareData
* @brief метод формирования значения топика из полей структуры
* @param none
* @return QByteArray
** ***********************************************************************/
QByteArray CTopicStmDoubleValue::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_VALUE, m_value );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
