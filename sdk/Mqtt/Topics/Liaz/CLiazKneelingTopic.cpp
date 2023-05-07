#include "CLiazKneelingTopic.h"

static const char * TOPIC = "KNEELING";

static const char * FIELD_VALUE = "Value";

static const QStringList STATES{
    "Not_active",
    "Lowering_active",
    "Kneeling_level_reached",
    "Lifting_active",
    "Kneeling_aborted",
    "Error",
    "Unknown"
};

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CLiazKneelingTopic::CLiazKneelingTopic() :
    CGrLiazTopic( TOPIC )
{ }

CLiazKneelingTopic::CLiazKneelingTopic( const CLiazKneelingTopic::EKneelingStates state ) :
    CGrLiazTopic( TOPIC ),
    m_state{ state }
{ }

/** ***********************************************************************
 * @fn setState
 ** ***********************************************************************/
void CLiazKneelingTopic::setState(const CLiazKneelingTopic::EKneelingStates value)
{
    m_state = value;
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CLiazKneelingTopic::prepareData() const
{
    QJsonObject jsonMainObj{ { FIELD_VALUE, STATES.value( m_state ) } };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CLiazKneelingTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_VALUE ) )
        {
            QString indexStr( jsonMainObj.value( FIELD_VALUE ).toString() );
            int index{ STATES.indexOf( indexStr ) };
            if ( index >= static_cast< int >( EKneelingStates::Kneeling_notActive ) &&
                 index <= static_cast< int >( EKneelingStates::Kneeling_error ) )
            {
                m_state = static_cast< EKneelingStates >( index );
            }
            else
            {
                m_state = EKneelingStates::Kneeling_unknown;
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

