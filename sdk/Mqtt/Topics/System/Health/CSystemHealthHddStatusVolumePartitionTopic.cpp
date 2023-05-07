#include "CSystemHealthHddStatusVolumePartitionTopic.h"

static const char * FIELD_PARTITION = "partition";
static const char * FIELD_ISMOUNTED = "is_mounted";
static const char * FIELD_MOUNTPOINT = "mount_point";
static const char * FIELD_TOTALVOLUME = "total_space";
static const char * FIELD_FREEVOLUME = "free_space";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CSystemHealthHddStatusVolumePartitionTopic::CSystemHealthHddStatusVolumePartitionTopic( const QString & volume,
                                                                                        const QString & partition ) :
    CGrSystemHealthHddStatusTopic( volume + "/" + partition )
{ }

CSystemHealthHddStatusVolumePartitionTopic::~CSystemHealthHddStatusVolumePartitionTopic()
{ }

void CSystemHealthHddStatusVolumePartitionTopic::setPartition(const QString & value)
{
    m_partition = value;
}

void CSystemHealthHddStatusVolumePartitionTopic::setIsMounted(const int value)
{
    m_isMounted = value;
}

void CSystemHealthHddStatusVolumePartitionTopic::setMountPoint(const QString & value)
{
    m_mountPoint = value;
}

void CSystemHealthHddStatusVolumePartitionTopic::setTotalVolume(const QString & value)
{
    m_totalVolume = value;
}

void CSystemHealthHddStatusVolumePartitionTopic::setFreeVolume(const QString & value)
{
    m_freeVolume = value;
}

/** ***********************************************************************
 * @brief prepareData
 ** ***********************************************************************/
const QByteArray CSystemHealthHddStatusVolumePartitionTopic::prepareData() const
{
    QJsonObject jsonMainObj{ { FIELD_PARTITION, m_partition },
                             { FIELD_ISMOUNTED, m_isMounted },
                             { FIELD_MOUNTPOINT, m_mountPoint },
                             { FIELD_TOTALVOLUME, m_totalVolume },
                             { FIELD_FREEVOLUME, m_freeVolume } };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @brief parseData
 ** ***********************************************************************/
bool CSystemHealthHddStatusVolumePartitionTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_PARTITION ) &&
             jsonMainObj.contains( FIELD_ISMOUNTED ) &&
             jsonMainObj.contains( FIELD_MOUNTPOINT ) )
        {
            m_partition = jsonMainObj.value( FIELD_PARTITION ).toString();
            m_isMounted = jsonMainObj.value( FIELD_ISMOUNTED ).toInt();
            m_mountPoint = jsonMainObj.value( FIELD_MOUNTPOINT ).toString();
            if ( jsonMainObj.contains( FIELD_TOTALVOLUME ) )
            {
                m_totalVolume = jsonMainObj.value( FIELD_TOTALVOLUME ).toString();
            }
            if ( jsonMainObj.contains( FIELD_FREEVOLUME ) )
            {
                m_freeVolume = jsonMainObj.value( FIELD_FREEVOLUME ).toString();
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
