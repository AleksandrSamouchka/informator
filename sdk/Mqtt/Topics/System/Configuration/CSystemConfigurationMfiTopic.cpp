#include "CSystemConfigurationMfiTopic.h"

static const char *SUBTOPICNAME = "MFI";

static const char * FIELD_NUMBER = "Number";
static const char * FIELD_TYPE = "Type";
static const char * FIELD_POLARITY = "Polarity";


CSystemConfigurationMfiTopic::CSystemConfigurationMfiTopic() :
    CSystemConfigurationTopic( SUBTOPICNAME )
{
}

CSystemConfigurationMfiTopic::CSystemConfigurationMfiTopic(const QByteArray & data) :
    CSystemConfigurationTopic( SUBTOPICNAME, data )
{

}

CSystemConfigurationMfiTopic::~CSystemConfigurationMfiTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CSystemConfigurationMfiTopic::getEtalonJson()
{
    QJsonArray jsonArr{};
    for ( int i = 0; i < 3; i ++ )
    {
        QJsonObject jsonObj( { { FIELD_NUMBER, i },
                               { FIELD_TYPE, EMFIType::FREQ_IN },
                               { FIELD_POLARITY, EMFIActivePolarity::ACTIVE_1 } } );
        jsonArr.append( jsonObj );
    }
    return QJsonDocument( jsonArr ).toJson( QJsonDocument::Compact );
}
#endif

/** ***********************************************************************
 * @fn setInputSetting
 ** ***********************************************************************/
void CSystemConfigurationMfiTopic::setInputSetting( const int number,
                                                    const EMFIType type,
                                                    const EMFIActivePolarity polarity )
{
    if ( number >= 0 && number < MFI_COUNT )
    {
        TMFISettings mfiSett{};
        mfiSett.MFIType = type;
        mfiSett.MFIActivePolarity = polarity;
        m_mfiList.insert( number, mfiSett );
    }
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CSystemConfigurationMfiTopic::prepareData() const
{
    QJsonArray jsonArr{};
    for ( const auto & i : m_mfiList.keys() )
    {
        QJsonObject jsonObj( { { FIELD_NUMBER, i },
                               { FIELD_TYPE, m_mfiList.value( i ).MFIType },
                               { FIELD_POLARITY, m_mfiList.value( i ).MFIActivePolarity } } );
        jsonArr.append( jsonObj );
    }
    return QJsonDocument( jsonArr ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CSystemConfigurationMfiTopic::parseData(const QByteArray & data)
{
    m_mfiList.clear();
    bool res{ false };
    for ( const auto i : QJsonDocument::fromJson( data ).array() )
    {
        const QJsonObject jsonObj( i.toObject() );
        if ( jsonObj.contains( FIELD_NUMBER ) &&
             jsonObj.contains( FIELD_TYPE ) &&
             jsonObj.contains( FIELD_POLARITY )
         )
        {
            TMFISettings mfiSett{};
            mfiSett.MFIType = static_cast< EMFIType >( jsonObj.value( FIELD_TYPE ).toInt() );
            mfiSett.MFIActivePolarity = static_cast< EMFIActivePolarity >( jsonObj.value( FIELD_POLARITY ).toInt() );
            m_mfiList.insert( jsonObj.value( FIELD_NUMBER ).toInt(), mfiSett );
            res = true;
        }
        else
        {
            PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект содержит неизвестный список полей: " + jsonObj.keys().join( "," ) );
        }
    }
    return setValidState( res );
}

