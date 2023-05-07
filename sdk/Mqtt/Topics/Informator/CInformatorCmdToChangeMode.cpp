#include "CInformatorCmdToChangeMode.h"


static const char *SUBTOPICNAME = "CMD_TO_CHANGE_MODE";

static const char *FIELD_MODE = "Mode";

static const QStringList MODES_LIST {
    "Auto",
    "Manual"
};


CInformatorCmdToChangeMode::CInformatorCmdToChangeMode( ) :
    CGrInformatorTopic( SUBTOPICNAME ),
    m_mode{ EModes::Mode_auto }
{
}

CInformatorCmdToChangeMode::CInformatorCmdToChangeMode( EModes mode ) :
    CGrInformatorTopic( SUBTOPICNAME ),
    m_mode{ mode }
{
    prepareData();
}

CInformatorCmdToChangeMode::CInformatorCmdToChangeMode( const QByteArray & data ) :
    CGrInformatorTopic( SUBTOPICNAME, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CInformatorCmdToChangeMode::getEtalonJson()
{
    QJsonObject jsonMainObj{ { FIELD_MODE, MODES_LIST.value( static_cast< int >( EModes::Mode_manual ) )} };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CInformatorCmdToChangeMode::setMode(CInformatorCmdToChangeMode::EModes mode)
{
    m_mode = mode;
}

const QByteArray CInformatorCmdToChangeMode::prepareData() const
{
    QJsonObject jsonMainObj{ { FIELD_MODE, MODES_LIST.value( static_cast< int >( m_mode ) ) } };
//    QByteArray jsonData{ 
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
// };
//    return setData( /*jsonData*/jsonMainObj );
}

bool CInformatorCmdToChangeMode::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_MODE ) )
        {
            const int index{ MODES_LIST.indexOf( jsonMainObj.value( FIELD_MODE ).toString() ) };
            if ( index >= 0 )
            {
                m_mode = static_cast< EModes >( index );
            }
            else
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Неизвестное содержимое [" + jsonMainObj.value( FIELD_MODE ).toString()
                                            + "] для поля[" + FIELD_MODE + "]" );
                m_mode = EModes::Mode_auto;
            }

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

