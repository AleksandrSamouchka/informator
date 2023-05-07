#include "CAudioMuteDriverTopic.h"                           
                                                               
CAudioMuteDriverTopic::CAudioMuteDriverTopic() :
    CAudioMuteTopic("MUTE_DRIVER")
{
}

CAudioMuteDriverTopic::~CAudioMuteDriverTopic()
{                                                              
}

#ifdef MNT_TOPICS_TEST
QByteArray CAudioMuteDriverTopic::getEtalonJson()
{
    QJsonObject jsonMainObj({
                                { "Value", 1 }
                            });

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif
