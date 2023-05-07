#include "CTopicStringValue.h"

namespace
{
    const QString FIELD_VALUE{ QString::fromLatin1( "Value" ) };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicStringValue::CTopicStringValue(const QString & value, const QByteArray json ) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_unknown ),
    m_value{ value }
{ }

CTopicStringValue::CTopicStringValue() :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_unknown )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param const CTopicValueEntry
 * @return bool
 ** ***********************************************************************/
bool CTopicStringValue::initFromData(const CTopicValueEntry & value)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_VALUE ) )
        {
            m_value = jsonMainObj.value( FIELD_VALUE ).toString();
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
QByteArray CTopicStringValue::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_VALUE, m_value );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
