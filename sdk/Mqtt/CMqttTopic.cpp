#include <QDebug>
#include "CMqttTopic.h"


static const char * FIELD_VALUE = "Value";

/** ***********************************************************************
 * @fn Конструкторы
 ** ***********************************************************************/
CMqttTopic::CMqttTopic() :
    QObject()
{ 
}

CMqttTopic::CMqttTopic(const CMqttTopic & other):
    QObject(),
    m_topicName{ other.topicName() },
    m_data{ QByteArray( other.data() ) },
    m_valid{ other.isValid() }/*,
    m_mqttConnectionName{ other.mqttConnectionName() }*/
{ 
}

CMqttTopic::CMqttTopic( const QString &topicName ) :
    QObject(),
    m_topicName( topicName )
{ 
}

CMqttTopic::CMqttTopic( const QString & topicName, const QByteArray & data ):
    QObject(),
    m_topicName( topicName )
{
    initFromData( data );
}

CMqttTopic::~CMqttTopic()
{
}

/** ***********************************************************************
 * @fn setMqttConnectionName
 ** ***********************************************************************/
void CMqttTopic::setMqttConnectionName(const QString & connName)
{
    m_mqttConnectionName = connName;
}

/** ***********************************************************************
 * @fn initFromData
 ** ***********************************************************************/
bool CMqttTopic::initFromData( const QByteArray & data )
{
    m_data = data;
    return parseData( /*setData(*/m_data/*)*/ );
}

/** ***********************************************************************
 * @fn buildMessage
 ** ***********************************************************************/
const CMqttMessage CMqttTopic::buildMessage( bool retain /*= false*/) const
{
    if(topicName().isEmpty())
    {
qWarning() << "Error: Topic Name is empty";
    } 
    CMqttMessage message( topicName(), prepareData(), retain );
    return message;
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CMqttTopic::prepareData() const
{
    return m_data;
}

/** ***********************************************************************
 * @fn prepareFromInt
 ** ***********************************************************************/
const QByteArray CMqttTopic::prepareFromInt( int value ) const
{
    QJsonObject jsonObj{ { FIELD_VALUE, value } };
    return QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact) ;
}

/** ***********************************************************************
 * @fn parseData
 ** ***********************************************************************/
bool CMqttTopic::parseData(const QByteArray & data)
{
    return setValidState( ! data.isEmpty() );
}

/** ***********************************************************************
 * @fn parseToInt
 ** ***********************************************************************/
int CMqttTopic::parseToInt( const QByteArray & data, bool * res )
{
    int value{ -1 };
    bool result{ false };
    QJsonParseError jsonErr{};
    QJsonObject jsonObj( QJsonDocument::fromJson( data, & jsonErr ).object() );
    if ( jsonErr.error != QJsonParseError::NoError )
    {
        printJsonParseErrorMessage( data, jsonErr );
    }
    else
    {
        if ( ! jsonObj.contains( FIELD_VALUE ) )
        {
            printUnknownFieldsListMessage( jsonObj.keys() );
        }
        else
        {
            value = jsonObj.value( FIELD_VALUE ).toInt( -1 );
            result = true;
        }
    }

    setValidState( result );
    if ( res != nullptr )
    {
        *res = result;
    }
    return value;
}

/** ***********************************************************************
 * @fn методы отображения ошибок разбора данных
 ** ***********************************************************************/
void CMqttTopic::printJsonParseErrorMessage( const QByteArray & data,
                                             const QJsonParseError & error ) const
{
    PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName()
                                + "]: Ошибка разбора входного сообщения! Ошибка["
                                + QString::number( error.error ) + "], cимвол №["
                                + QString::number( error.offset ) + "], сообщение["
                                + QString::fromUtf8( data ) + "]" );
}
void CMqttTopic::printEmptyJsonMessage() const
{
    PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName() + "]: Пустой JSON" );
}     
void CMqttTopic::printUnknownFieldsListMessage(const QStringList &filedsList) const
{
    PRINT_CONSOLE_MESSAGE_WARN( "Топик[" + topicName() + "]: JSON объект содержит неизвестный список полей: "
                                + filedsList.join( "," ) );
}


