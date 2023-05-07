#include "CAudioMuteSpeakingCityTopic.h"                           
                                                               
CAudioMuteSpeakingCityTopic::CAudioMuteSpeakingCityTopic() :
    CAudioMuteTopic("MUTE_SPEAKING_CITY")
{
}

CAudioMuteSpeakingCityTopic::~CAudioMuteSpeakingCityTopic()
{                                                              
}

#ifdef MNT_TOPICS_TEST
QByteArray CAudioMuteSpeakingCityTopic::getEtalonJson()
{
    QJsonObject jsonMainObj({
                                { "Value", 1 }
                            });

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif
