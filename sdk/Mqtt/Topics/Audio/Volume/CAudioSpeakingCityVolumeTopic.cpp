#include "CAudioSpeakingCityVolumeTopic.h"                           
                                                               
CAudioSpeakingCityVolumeTopic::CAudioSpeakingCityVolumeTopic() :
    CAudioVolumeTopic("SPEAKING_CITY_VOLUME")
{
}

CAudioSpeakingCityVolumeTopic::~CAudioSpeakingCityVolumeTopic()
{                                                              
}

#ifdef MNT_TOPICS_TEST
QByteArray CAudioSpeakingCityVolumeTopic::getEtalonJson()
{
    QJsonObject jsonMainObj({
                                { "Sender", "testSender" },
                                { "Value", 80 },
                                { "NeedToSave", true }
                            });

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

