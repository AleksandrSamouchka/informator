#include "CAudioSalonVolumeTopic.h"                           
                                                               
CAudioSalonVolumeTopic::CAudioSalonVolumeTopic() :
    CAudioVolumeTopic("SALON_VOLUME")
{
}

CAudioSalonVolumeTopic::~CAudioSalonVolumeTopic()
{                                                              
}

#ifdef MNT_TOPICS_TEST
QByteArray CAudioSalonVolumeTopic::getEtalonJson()
{
    QJsonObject jsonMainObj({
                                { "Sender", "testSender" },
                                { "Value", 70 },
                                { "NeedToSave", true }
                            });

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif

