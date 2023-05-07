#include "CAudioMuteSalonTopic.h"                           
                                                               
CAudioMuteSalonTopic::CAudioMuteSalonTopic() :
    CAudioMuteTopic("MUTE_SALON")
{
}

CAudioMuteSalonTopic::~CAudioMuteSalonTopic()
{                                                              
}

#ifdef MNT_TOPICS_TEST
QByteArray CAudioMuteSalonTopic::getEtalonJson()
{
    QJsonObject jsonMainObj({
                                { "Value", 1 }
                            });

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif
