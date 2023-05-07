#include "CNavigationRawNmeaTopic.h"

static const char * TOPIC = "RAW_NMEA";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CNavigationRawNmeaTopic::CNavigationRawNmeaTopic() :
    CGrNavigationTopic( TOPIC )
{ }

CNavigationRawNmeaTopic::CNavigationRawNmeaTopic(const QString & rawData) :
    CGrNavigationTopic( TOPIC ),
    m_rawData{ rawData }
{
    prepareData();
}

CNavigationRawNmeaTopic::CNavigationRawNmeaTopic(const QByteArray & data) :
    CGrNavigationTopic( TOPIC, data )
{ }

#ifdef MNT_TOPICS_TEST
QByteArray CNavigationRawNmeaTopic::getEtalonJson()
{
    return QByteArray( "RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW_RAW" );
}
#endif

void CNavigationRawNmeaTopic::setRawDataString(const QString & rawData)
{
    m_rawData = rawData;
}

const QByteArray CNavigationRawNmeaTopic::prepareData() const
{
//    return setData( 
    return m_rawData.toUtf8();
// );
}

bool CNavigationRawNmeaTopic::parseData(const QByteArray & data)
{
    m_rawData = QString::fromUtf8( data );
    return setValidState( true );
}
