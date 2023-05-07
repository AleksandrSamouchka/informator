#include "CSystemHealthHddStatusVolumeTopic.h"

static const char *FIELD_PARTITIONS = "partitions";
static const char *FIELD_SYSTEM = "system";
static const char *FIELD_MANUFACTURER = "manufacturer";
static const char *FIELD_MODEL = "model";
static const char *FIELD_SN = "sn";
static const char *FIELD_POWERONHOURS = "power_on_hours";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CSystemHealthHddStatusVolumeTopic::CSystemHealthHddStatusVolumeTopic( const QString & volume ) :
    CGrSystemHealthHddStatusTopic( volume )
{ }

CSystemHealthHddStatusVolumeTopic::~CSystemHealthHddStatusVolumeTopic()
{ }

void CSystemHealthHddStatusVolumeTopic::setPartitions(const QString & value)
{
    m_partition = value;
}

void CSystemHealthHddStatusVolumeTopic::setSystem(const int value)
{
    m_system = value;
}

void CSystemHealthHddStatusVolumeTopic::setManufacturer(const QString & value)
{
    m_manufacturer = value;
}

void CSystemHealthHddStatusVolumeTopic::setModel(const QString & value)
{
    m_model = value;
}

void CSystemHealthHddStatusVolumeTopic::setSn(const QString & value)
{
    m_sn = value;
}

void CSystemHealthHddStatusVolumeTopic::setPowerOnHours(const QString & value)
{
    m_powerOnHours = value;
}

/** ***********************************************************************
 * @brief prepareData
 ** ***********************************************************************/
const QByteArray CSystemHealthHddStatusVolumeTopic::prepareData() const
{
    QJsonObject jsonMainObj{ { FIELD_PARTITIONS, m_partition },
                             { FIELD_SYSTEM, m_system },
                             { FIELD_MANUFACTURER, m_manufacturer },
                             { FIELD_MODEL, m_model },
                             { FIELD_SN, m_sn },
                             { FIELD_POWERONHOURS, m_powerOnHours } };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @brief parseData
 ** ***********************************************************************/
bool CSystemHealthHddStatusVolumeTopic::parseData( const QByteArray & data )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_PARTITIONS ) )
        {
            m_partition = jsonMainObj.value( FIELD_PARTITIONS ).toString();
            if ( jsonMainObj.contains( FIELD_SYSTEM ) )
            {
                m_system = jsonMainObj.value( FIELD_SYSTEM ).toInt();
            }
            if ( jsonMainObj.contains( FIELD_MANUFACTURER ) )
            {
                m_manufacturer = jsonMainObj.value( FIELD_MANUFACTURER ).toString();
            }
            if ( jsonMainObj.contains( FIELD_MODEL ) )
            {
                m_model = jsonMainObj.value( FIELD_MODEL ).toString();
            }
            if ( jsonMainObj.contains( FIELD_SN ) )
            {
                m_sn = jsonMainObj.value( FIELD_SN ).toString();
            }
            if ( jsonMainObj.contains( FIELD_POWERONHOURS ) )
            {
                m_powerOnHours = jsonMainObj.value( FIELD_POWERONHOURS ).toString();
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
