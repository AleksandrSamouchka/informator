#include "CSystemHealthHddPrepareTopic.h"

static const char * TOPIC = "HDD_PREPARE";

static const char *FIELD_VOLUME = "volume";
static const char *FIELD_PREPARED = "prepared";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CSystemHealthHddPrepareTopic::CSystemHealthHddPrepareTopic() :
    CGrSystemHealthTopic( TOPIC )
{ }

CSystemHealthHddPrepareTopic::~CSystemHealthHddPrepareTopic()
{ }

/** ***********************************************************************
 * @brief setVolume
 ** ***********************************************************************/
void CSystemHealthHddPrepareTopic::setVolume(const QString & value)
{
    m_volume = value;
}

/** ***********************************************************************
 * @brief setPrepared
 ** ***********************************************************************/
void CSystemHealthHddPrepareTopic::setPrepared(const int value)
{
    m_prepared = value;
}

/** ***********************************************************************
 * @brief prepareData
 ** ***********************************************************************/
const QByteArray CSystemHealthHddPrepareTopic::prepareData() const
{
    QJsonObject jsonMainObj{ { FIELD_VOLUME, m_volume },
                             { FIELD_PREPARED, m_prepared } };
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/** ***********************************************************************
 * @brief parseData
 ** ***********************************************************************/
bool CSystemHealthHddPrepareTopic::parseData( const QByteArray & data )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_VOLUME )
             && jsonMainObj.contains( FIELD_PREPARED ) )
        {
            m_volume = jsonMainObj.value( FIELD_VOLUME ).toString();
            m_prepared = jsonMainObj.value( FIELD_PREPARED ).toInt();
            res = true;
        }
        else
        {
            printUnknownFieldsListMessage( jsonMainObj.keys() );
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    return setValidState( res );
}
