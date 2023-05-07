#include "CGuiCvsStateTopic.h"

static const char * TOPIC = "CVS_STATE";

static const char *FIELD_INWORK = "in_work";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CGuiCvsStateTopic::CGuiCvsStateTopic():
    CGrGuiTopic( TOPIC )
{ }

CGuiCvsStateTopic::CGuiCvsStateTopic( const int inWork ):
    CGrGuiTopic( TOPIC ),
    m_inWork{ inWork }
{ }

CGuiCvsStateTopic::CGuiCvsStateTopic( const QByteArray &data ) :
    CGrGuiTopic( TOPIC, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CGuiCvsStateTopic::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_INWORK, 1 } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CGuiCvsStateTopic::setInWork(const int value)
{
    m_inWork = value;
}

const QByteArray CGuiCvsStateTopic::prepareData() const
{
    QJsonObject jsonMainObj( { { FIELD_INWORK, m_inWork } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

bool CGuiCvsStateTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_INWORK ) )
        {
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
    return setValidState( res );
}


