#include "CStmRoMfiTopic.h"

static const char * TOPIC = "MFI";

static const char *FIELD_MFI = "MFI";
static const char *FIELD_TYPE = "Type";
static const char *FIELD_VALUE = "Value";

static const char *TYPE_ANALOG = "analog";
static const char *TYPE_DIGITAL = "digital";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CStmRoMfiTopic::CStmRoMfiTopic() :
    CGrStmRoTopic( TOPIC )
{ }

CStmRoMfiTopic::~CStmRoMfiTopic()
{ }

/** ***********************************************************************
 * @fn appendInputState
 ** ***********************************************************************/
void CStmRoMfiTopic::appendInputState(const CStmRoMfiTopic::SInputState & value)
{
    m_sensors.append( value );
}

/** ***********************************************************************
 * @fn setInputStateList
 ** ***********************************************************************/
void CStmRoMfiTopic::setInputStateList(const QList<CStmRoMfiTopic::SInputState> & value)
{
    m_sensors = value;
}

/** ***********************************************************************
 * @fn clearInputStateList
 ** ***********************************************************************/
void CStmRoMfiTopic::clearInputStateList()
{
    m_sensors.clear();
}

/** ***********************************************************************
 * @fn prepareData
 ** ***********************************************************************/
const QByteArray CStmRoMfiTopic::prepareData() const
{
    QJsonArray jsonArr{};
    for ( const auto & i : m_sensors )
    {
        QJsonObject jsonObj{ { FIELD_TYPE, i.m_type == SInputState::ETypes::Type_analog
                              ? TYPE_ANALOG : TYPE_DIGITAL },
                             { FIELD_VALUE, i.m_value } };
        jsonArr.append( jsonObj );
    }
    QJsonObject jsonMainObj{ { FIELD_MFI, jsonArr } };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

    /** ***********************************************************************
     * @fn parseData
     ** ***********************************************************************/
bool CStmRoMfiTopic::parseData(const QByteArray & data)
{
    m_sensors.clear();
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_MFI ) )
        {
            for ( const auto i : jsonMainObj.value( FIELD_MFI ).toArray() )
            {
                if ( i.toObject().contains( FIELD_TYPE )
                     && i.toObject().contains( FIELD_VALUE ) )
                {
                    SInputState sensor{};
                    sensor.m_value = i.toObject().value( FIELD_VALUE ).toDouble();
                    const QString type{ i.toObject().value( FIELD_TYPE ).toString() };
                    if ( type == TYPE_ANALOG )
                    {
                        sensor.m_type = SInputState::ETypes::Type_analog;
                    }
                    else if ( type == TYPE_DIGITAL )
                    {
                        sensor.m_type = SInputState::ETypes::Type_digital;
                    }
                    else
                    {
                        PRINT_CONSOLE_MESSAGE_WARN( "Неизвестный тип датчика [" + type + "]" );
                    }
                    m_sensors.append( sensor );
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "JSON объект списка содержит неизвестный список полей: " + i.toObject().keys().join( "," ) );
                }
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


