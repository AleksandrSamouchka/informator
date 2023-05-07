#include "CValidatorsStateTopic.h"

static const char * TOPIC = "STATE";

static const char *FIELD_ENABLED = "enabled";
static const char *FIELD_INWORK = "in_work";

CValidatorsStateTopic::CValidatorsStateTopic( const int enabled,
                                              const int inWork ) :
    CGrValidatorsTopic( TOPIC ),
    m_enabled{ enabled },
    m_inWork{ inWork }
{ }

CValidatorsStateTopic::CValidatorsStateTopic() :
    CGrValidatorsTopic( TOPIC )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CValidatorsStateTopic::getEtalonJson()
{
    QJsonObject jsonMainObj( { { FIELD_ENABLED, 1 },
                                     { FIELD_INWORK, 1 } } );
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

void CValidatorsStateTopic::setEnabled(const int value)
{
    m_enabled = value;
}

void CValidatorsStateTopic::setInWork(const int value)
{
    m_inWork = value;
}

const QByteArray CValidatorsStateTopic::prepareData() const
{
    QJsonObject jsonMainObj( { { FIELD_ENABLED, m_enabled },
                                     { FIELD_INWORK, m_inWork } } );
//    QByteArray jsonData{ 
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
// };
//    return setData( /*jsonData*/jsonMainObj );
}

bool CValidatorsStateTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
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
    return setValidState( res );
}
