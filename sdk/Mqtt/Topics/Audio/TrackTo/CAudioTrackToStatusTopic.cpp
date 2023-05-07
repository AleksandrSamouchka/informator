#include "CAudioTrackToStatusTopic.h"

//static const char * FIELD_STATUS = "Status";

static const char *QUEUED_STATUS = "Queued";
static const char *PLAYING_STATUS = "Playing";
static const char *FINISHED_STATUS = "Finished";
static const char *UNKNOWN_STATUS = "Unknown";


static const QStringList STATUSES{ QUEUED_STATUS, PLAYING_STATUS, FINISHED_STATUS };

CAudioTrackToStatusTopic::CAudioTrackToStatusTopic() :
    CAudioTrackToTopic("STATUS")
{
}

CAudioTrackToStatusTopic::~CAudioTrackToStatusTopic()
{                                                              
}

bool CAudioTrackToStatusTopic::parseData( const QByteArray & data )
{
    bool res = false;
    QStringList keys = {
        CAudioTopic::FIELD_SENDER,
        CAudioTopic::FIELD_STATUS,
        CAudioTopic::FIELD_TARGET,
        CAudioTopic::FIELD_UUID
    };


    QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.keys() == keys)
        {
            setField( FIELD_SENDER, jsonMainObj.value( FIELD_SENDER ).toVariant() );
            setField( FIELD_STATUS, jsonMainObj.value( FIELD_STATUS ).toVariant() );
            setField( FIELD_TARGET, jsonMainObj.value( FIELD_TARGET ).toVariant() );
            setField( FIELD_UUID, jsonMainObj.value( FIELD_UUID ).toVariant() );
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

//    if ( res == false )
//    {
        return res;
//    }
//    return CAudioTrackToTopic::parseData( QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) );
}

CAudioTrackToStatusTopic::EStatuses CAudioTrackToStatusTopic::stringToStatus(const QString &statusString) const
{
    EStatuses statusValue = EStatuses::unknown;

    if(statusString == QUEUED_STATUS)
    {
        statusValue = EStatuses::queued;
    }
    else if(statusString == PLAYING_STATUS)
    {
        statusValue = EStatuses::playing;
    }
    else if(statusString == FINISHED_STATUS)
    {
        statusValue = EStatuses::finished;
    }

    return statusValue;
}

void CAudioTrackToStatusTopic::setField(const char *name, EStatuses status)
{
    if(status == EStatuses::queued)
    {
        setField(name, QUEUED_STATUS);
    }
    else if(status == EStatuses::playing)
    {
        setField(name, PLAYING_STATUS);
    }
    else if(status == EStatuses::finished)
    {
        setField(name, FINISHED_STATUS);
    }
    else
    {
        setField(name, UNKNOWN_STATUS);
    }
//    CAudioTopic::setField(name, static_cast<int>(action));
}


#ifdef MNT_TOPICS_TEST
QByteArray CAudioTrackToStatusTopic::getEtalonJson()
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_STATUS, PLAYING_STATUS );

    return CAudioTrackToTopic::getEtalonJson( QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) );
}
#endif

