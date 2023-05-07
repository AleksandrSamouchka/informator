#include "CIncarnetInspectorDeviceStateTopic.h"

static const char * TOPIC = "DEVICE_STATES";

static const char *FIELD_MAC = "mac";
static const char *FIELD_CHANNELS = "channels";
static const char *FIELD_IS_ACTIVE = "is_active";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CIncarnetInspectorDeviceStateTopic::CIncarnetInspectorDeviceStateTopic() :
    CGrIncarnetInspectorTopic( TOPIC )
{ }

CIncarnetInspectorDeviceStateTopic::CIncarnetInspectorDeviceStateTopic(const QByteArray & data) :
    CGrIncarnetInspectorTopic( TOPIC, data )
{ }

void CIncarnetInspectorDeviceStateTopic::clearDeviceStates()
{
    m_devices.clear();
}

void CIncarnetInspectorDeviceStateTopic::appendDeviceState(const CIncarnetInspectorDeviceStateTopic::SDeviceInfo & info)
{
    m_devices.append( info );
}

void CIncarnetInspectorDeviceStateTopic::setDeviceStates(const QList<CIncarnetInspectorDeviceStateTopic::SDeviceInfo> & states)
{
    m_devices = states;
}

const QByteArray CIncarnetInspectorDeviceStateTopic::prepareData() const
{
    QJsonArray jsonArr{};
    for ( const auto & i : m_devices )
    {
        QJsonObject jsonObj{ { FIELD_MAC, i.m_mac },
                             { FIELD_CHANNELS, QJsonArray::fromStringList( i.m_channels ) },
                             { FIELD_IS_ACTIVE, i.m_isActive } };
        jsonArr.append( jsonObj );
    }
    return QJsonDocument( jsonArr ).toJson( QJsonDocument::Compact );
}

bool CIncarnetInspectorDeviceStateTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    m_devices.clear();
    QJsonDocument jsonDoc( QJsonDocument::fromJson( data ) );
    if ( jsonDoc.isArray() )
    {
        res = true;
        const QJsonArray jsonArr( jsonDoc.array() );
        for ( const auto i : jsonArr )
        {
            QJsonObject jsonObj( i.toObject() );
            if ( jsonObj.contains( FIELD_MAC )
                 && jsonObj.contains( FIELD_CHANNELS )
                 && jsonObj.contains( FIELD_IS_ACTIVE ) )
            {
                QStringList channels{};
                for ( const auto y : jsonObj.value( FIELD_CHANNELS ).toArray() )
                {
                    channels.append( y.toString() );
                }
                m_devices.append( SDeviceInfo( jsonObj.value( FIELD_MAC ).toString(),
                                               channels,
                                               jsonObj.value( FIELD_IS_ACTIVE ).toBool() ) );
            }
            else
            {
                printUnknownFieldsListMessage( jsonObj.keys() );
            }
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    return setValidState( res );
}
