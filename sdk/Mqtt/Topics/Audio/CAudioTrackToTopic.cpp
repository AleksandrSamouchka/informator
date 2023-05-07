#include "CAudioTrackToTopic.h"                           

//static const char *FIELD_SENDER = "Sender";
//static const char *FIELD_ACTION = "Action";
//static const char *FIELD_TRACK = "Track";
//static const char *FIELD_UUID = "Uuid";

static const QStringList ACTIONS{ "Start", "Stop" };

static const char *TOPIC_NAME = "TRACK_TO";

CAudioTrackToTopic::CAudioTrackToTopic():
    CAudioTopic(TOPIC_NAME)
{
    setField(QString(FIELD_DELAY), 0);
    setField(QString(FIELD_PRIORITY), 0);
}

CAudioTrackToTopic::CAudioTrackToTopic(const QString &subTopicName) :
    CAudioTopic(QString("%1/%2").arg(TOPIC_NAME).arg(subTopicName))
{
    setField(QString(FIELD_DELAY), 0);
    setField(QString(FIELD_PRIORITY), 0);
}

CAudioTrackToTopic::~CAudioTrackToTopic()
{                                                              
}

CAudioTrackToTopic::EAction CAudioTrackToTopic::stringToAction(const QString &actionString) const
{
    EAction actValue = EAction::unknown;

    if(actionString == "Start")
    {
        actValue = EAction::start;
    }
    else if(actionString == "Stop")
    {
        actValue = EAction::stop;
    }

    return actValue;
}

bool CAudioTrackToTopic::parseData( const QByteArray &data )
{
    bool res = false;

    QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    /*
    QStringList keys = {
        CAudioTopic::FIELD_ACTION,
        CAudioTopic::FIELD_DELAY,
        CAudioTopic::FIELD_PRIORITY,
        CAudioTopic::FIELD_SENDER,
        CAudioTopic::FIELD_TARGET,
        CAudioTopic::FIELD_TRACK,
        CAudioTopic::FIELD_UUID
    };
    */

    if ( ! jsonMainObj.isEmpty() )
    {
        QStringList keys = jsonMainObj.keys();

        if (keys.contains(FIELD_ACTION) && keys.contains(FIELD_SENDER)
                && keys.contains(FIELD_TARGET) && keys.contains(FIELD_TRACK)
                && keys.contains(FIELD_UUID))
        {
            QString tmp = jsonMainObj.value( CAudioTopic::FIELD_SENDER ).toString();
            CAudioTopic::setField(CAudioTopic::FIELD_SENDER, tmp);
            tmp = jsonMainObj.value( CAudioTopic::FIELD_ACTION ).toString();
            EAction actValue = stringToAction(tmp); //EAction::unknown;
            if(actValue == EAction::unknown)
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Unknown track action: " +
                                            tmp + ". Expected: " +
                                            ACTIONS.join(","));
            }

            setField(CAudioTopic::FIELD_ACTION, actValue);

            CAudioTopic::setField(CAudioTopic::FIELD_TRACK, jsonMainObj.value( CAudioTopic::FIELD_TRACK ).toString());
            CAudioTopic::setField(CAudioTopic::FIELD_UUID, jsonMainObj.value( CAudioTopic::FIELD_UUID ).toString());
            CAudioTopic::setField(CAudioTopic::FIELD_TARGET, jsonMainObj.value( CAudioTopic::FIELD_TARGET ).toString());

            if (keys.contains(FIELD_DELAY))
                CAudioTopic::setField(CAudioTopic::FIELD_DELAY, jsonMainObj.value(CAudioTopic::FIELD_DELAY).toString());
            else
                CAudioTopic::setField(CAudioTopic::FIELD_DELAY, QString("0"));

            if (keys.contains(FIELD_PRIORITY))
                CAudioTopic::setField(CAudioTopic::FIELD_PRIORITY, jsonMainObj.value(CAudioTopic::FIELD_PRIORITY).toString());
            else
                CAudioTopic::setField(CAudioTopic::FIELD_PRIORITY, QString("0"));

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
    setValidState( res );
    return res;
}

void CAudioTrackToTopic::setField(const char *name, EAction action)
{
    if(action == EAction::start)
    {
        setField(name, "Start");
    }        
    else if(action == EAction::stop)
    {
        setField(name, "Stop");
    }        
    else 
    {
        setField(name, "Unknown");
    }        
//    CAudioTopic::setField(name, static_cast<int>(action));
}


//QByteArray CTopicAudioTrackToPlay::prepareData() const
const QByteArray CAudioTrackToTopic::prepareData() const
{
    QJsonObject jsonMainObj{};
    foreach(const QString key, m_fields.keys() )
    {
        if(key == CAudioTopic::FIELD_ACTION)
        {
            EAction actValue = getAction();
            if( actValue == EAction::start)
            {
                jsonMainObj.insert( CAudioTopic::FIELD_ACTION, "Start");
            }
            else
            {
                jsonMainObj.insert( CAudioTopic::FIELD_ACTION, "Stop");
            }
        }
        else
        {
            jsonMainObj.insert(key, m_fields[key].toString());
        }
    }

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

#ifdef MNT_TOPICS_TEST
QByteArray CAudioTrackToTopic::getEtalonJson( const QByteArray & data )
{
    QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        jsonMainObj.insert( CAudioTopic::FIELD_SENDER, "Salon" );
        jsonMainObj.insert( CAudioTopic::FIELD_SENDER, "testSender" );
        jsonMainObj.insert( CAudioTopic::FIELD_ACTION, "Start" );
        jsonMainObj.insert( CAudioTopic::FIELD_TRACK, "/home/mntconti/informator/Common/6.mp3" );
        jsonMainObj.insert( CAudioTopic::FIELD_UUID, "{96889723-c197-4537-87ff-d8905e6378f6}" );
    }
    else
    {
        printEmptyJsonMessage();
    }
    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif
