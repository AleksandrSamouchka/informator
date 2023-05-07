#include "CTopicSettingsSettingChangeCmd.h"

namespace
{
    const char *FIELD_MODULE = "Module";
    const char *FIELD_SETTINGNAME = "Setting_name";
    const QStringList FIELDS{
        FIELD_MODULE,
        FIELD_SETTINGNAME
    };
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicSettingsSettingChangeCmd::CTopicSettingsSettingChangeCmd( const QString &module, 
                                                                const QString &settingName ) :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Settings_settingChangeCmd ),
    m_module{ module },
    m_settingName{ settingName }
{ }

/** ***********************************************************************
 * @fn prepareData
 * @brief метод формирования значения топика из полей структуры
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QByteArray CTopicSettingsSettingChangeCmd::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_MODULE, m_module );
    jsonMainObj.insert( FIELD_SETTINGNAME, m_settingName );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicSettingsSettingChangeCmd::initFromData(const CTopicValueEntry &value)
{
    bool res{ false };
    if ( value.isValid() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
        if ( ! jsonMainObj.isEmpty() )
        {
            if ( jsonMainObj.keys() == FIELDS )
            {
                m_module = jsonMainObj.value( FIELD_MODULE ).toString();
                m_settingName = jsonMainObj.value( FIELD_SETTINGNAME ).toString();
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
