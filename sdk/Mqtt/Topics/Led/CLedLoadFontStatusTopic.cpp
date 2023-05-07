#include "CLedLoadFontStatusTopic.h"

static const char * TOPIC = "LOAD_FONT/STATUS";

static const char *FIELD_TARGET = "target";
static const char *FIELD_STATUS = "status";
static const char *FIELD_TOTAL_PIECE_COUNT = "total_piece_count";
static const char *FIELD_CURRENT_PIECE_NUMBER = "current_piece_number";

static const char *unknownStatus = "UNKNOWN";

static const QStringList STATUSES {
    unknownStatus,
    "IN_PROGRESS",
    "FINISHED",
    "FAILED",
};

static QString getStatusName( CLedLoadFontStatusTopic::EStatus value )
{
    return STATUSES.value( value, unknownStatus );
}

static CLedLoadFontStatusTopic::EStatus getStatusCode(const QString & value)
{
    int index = STATUSES.indexOf( value );
    if ( index == -1 )
    {
        return CLedLoadFontStatusTopic::EStatus::statusesCount;
    }
    else
    {
        return static_cast< CLedLoadFontStatusTopic::EStatus >( index );
    }
}

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CLedLoadFontStatusTopic::CLedLoadFontStatusTopic() :
    CGrLedTopic( TOPIC )
{ }

CLedLoadFontStatusTopic::CLedLoadFontStatusTopic(const QByteArray & data) :
    CGrLedTopic( TOPIC, data )
{ }

/*void CLedLoadFontStatusTopic::setTarget(CGrLedTopic::ETargets value)
{
    m_target = value;
}
*/

void CLedLoadFontStatusTopic::setStatus(const CLedLoadFontStatusTopic::EStatus value)
{
    m_status = value;
}

void CLedLoadFontStatusTopic::setTotalPieceCounter(const int value)
{
    m_totalPieceCount = value;
}

void CLedLoadFontStatusTopic::setCurrentPieceNumber(const int value)
{
    m_currentPieceNumber = value;
}

const QByteArray CLedLoadFontStatusTopic::prepareData() const
{
    QJsonObject jsonObj{ { FIELD_TARGET, CGrLedTopic::getTargetName( /*m_target*/getTarget() ) },
                         { FIELD_STATUS, getStatusName( m_status ) },
                         { FIELD_TOTAL_PIECE_COUNT, m_totalPieceCount },
                         { FIELD_CURRENT_PIECE_NUMBER, m_currentPieceNumber } };
    return QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact );
}

bool CLedLoadFontStatusTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_TARGET ) &&
             jsonMainObj.contains( FIELD_STATUS ) &&
             jsonMainObj.contains( FIELD_TOTAL_PIECE_COUNT ) &&
             jsonMainObj.contains( FIELD_CURRENT_PIECE_NUMBER ) )
        {
            /*m_target = */setTarget(CGrLedTopic::getTargetCode( jsonMainObj.value( FIELD_TARGET ).toString() ));
            m_status = getStatusCode( jsonMainObj.value( FIELD_STATUS ).toString() );
            m_totalPieceCount = jsonMainObj.value( FIELD_TOTAL_PIECE_COUNT ).toInt();
            m_currentPieceNumber = jsonMainObj.value( FIELD_CURRENT_PIECE_NUMBER ).toInt();
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
