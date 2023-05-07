#include "CSettingsSettingChangeTopic.h"

static const char * TOPIC_NAME = "SETTING_CHANGE";
static const char * FIELD_MODULE = "Module";
static const char * FIELD_SETTINGNAME = "Setting_name";

CSettingsSettingChangeTopic::CSettingsSettingChangeTopic( const QString & module,
                                                          const QString & settingName ) :
    CGrSettingsTopic( TOPIC_NAME ),
    m_module( module ),
    m_settingName( settingName )
{
    prepareData();
}

CSettingsSettingChangeTopic::CSettingsSettingChangeTopic( const QByteArray &data ) :
    CGrSettingsTopic( TOPIC_NAME, data )
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CSettingsSettingChangeTopic::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_MODULE, "testModule" },
                               { FIELD_SETTINGNAME, "testSettingName" } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CSettingsSettingChangeTopic::setModule(const QString & module)
{
    m_module = module;
}

void CSettingsSettingChangeTopic::setSettingName(const QString & settingName)
{
    m_settingName = settingName;
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CSettingsSettingChangeTopic::prepareData() const 
{
    QJsonObject jsonMainObj( { { FIELD_MODULE, m_module },
                               { FIELD_SETTINGNAME, m_settingName } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CSettingsSettingChangeTopic::parseData( const QByteArray & data )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_MODULE ) &&
             jsonMainObj.contains( FIELD_SETTINGNAME ) )
        {
            m_module = jsonMainObj.value( FIELD_MODULE ).toString();
            m_settingName = jsonMainObj.value( FIELD_SETTINGNAME ).toString();
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
    return setValidState( res );
}
