#include "CAudioMicVolumeTopic.h"                           
                                                               
CAudioMicVolumeTopic::CAudioMicVolumeTopic() :
    CAudioVolumeTopic("MIC_VOLUME")
{
}

CAudioMicVolumeTopic::~CAudioMicVolumeTopic()
{                                                              
}

#ifdef MNT_TOPICS_TEST
QByteArray CAudioMicVolumeTopic::getEtalonJson()
{
    QJsonObject jsonMainObj({
                                { "Sender", "testSender" },
                                { "Value", 60 },
                                { "NeedToSave", true }
                            });

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif
