#include "CTopicAudioVolume.h"

namespace
{
    const char *FIELD_SENDER = "Sender";
    const char *FIELD_VALUE = "Value";
    const char *FIELD_NEED_TO_SAVE = "NeedToSave";
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicAudioVolume::CTopicAudioVolume(const QString &sender, int value, bool needToSave ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Audio_salonVolume ),
    m_sender{ sender },
    m_value{ value },
    m_needToSave{ needToSave }
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicAudioVolume::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_VALUE ) )
        {
            m_sender = jsonMainObj.value( FIELD_SENDER ).toString();
            m_value = jsonMainObj.value( FIELD_VALUE ).toInt();
            if ( jsonMainObj.contains( FIELD_NEED_TO_SAVE ) )
            {
                m_needToSave = jsonMainObj.value( FIELD_NEED_TO_SAVE ).toInt();
            }
            res = true;
        }
        else
        {
//            PRINT_CONSOLE_MESSAGE_WARN( "JSON объект содержит неизвестный список полей: " + jsonMainObj.keys().join( "," ) );
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
QByteArray CTopicAudioVolume::prepareData()
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_SENDER, m_sender );
    jsonMainObj.insert( FIELD_VALUE, m_value );
    jsonMainObj.insert( FIELD_NEED_TO_SAVE, m_needToSave );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}
