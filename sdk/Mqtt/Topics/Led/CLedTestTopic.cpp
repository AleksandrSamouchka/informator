#include "CLedTestTopic.h"

static const char * TOPIC = "TEST";

static const char *FIELD_TARGET = "target";
static const char *FIELD_NUMBER_TEXT = "number_text";
static const char *FIELD_STRING_TEXT = "string_text";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CLedTestTopic::CLedTestTopic() :
    CGrLedTopic( TOPIC )
{ }

CLedTestTopic::CLedTestTopic(const CGrLedTopic::ETargets target) :
    CGrLedTopic( TOPIC )/*,
    m_target{ target }*/
{ 
    setTarget(target);
}

CLedTestTopic::CLedTestTopic(const QByteArray & data) :
    CGrLedTopic( TOPIC, data )
{ }

/*void CLedTestTopic::setTarget(const ETargets value)
{
    m_target = value;
}
*/
void CLedTestTopic::setNumberText(const QString & value)
{
    m_numberText = value;
}

void CLedTestTopic::setStringText(const QString & value)
{
    m_stringText = value;
}

const QByteArray CLedTestTopic::prepareData() const
{
    QJsonObject jsonObj{ { FIELD_TARGET, CGrLedTopic::getTargetName( /*m_target*/getTarget() ) },
                         { FIELD_NUMBER_TEXT, m_numberText },
                         { FIELD_STRING_TEXT, m_stringText } };
    return QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact );
}

bool CLedTestTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_TARGET ) &&
             jsonMainObj.contains( FIELD_NUMBER_TEXT ) &&
             jsonMainObj.contains( FIELD_STRING_TEXT ) )
        {
            /*m_target = */setTarget(CGrLedTopic::getTargetCode( jsonMainObj.value( FIELD_TARGET ).toString() ));
            m_numberText = jsonMainObj.value( FIELD_NUMBER_TEXT ).toString();
            m_stringText = jsonMainObj.value( FIELD_STRING_TEXT ).toString();
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
