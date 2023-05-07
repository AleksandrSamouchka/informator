#include "CAudioDriverVolumeTopic.h"

CAudioDriverVolumeTopic::CAudioDriverVolumeTopic() :
    CAudioVolumeTopic("DRIVER_VOLUME")
{
}

CAudioDriverVolumeTopic::~CAudioDriverVolumeTopic()
{
}

#ifdef MNT_TOPICS_TEST
QByteArray CAudioDriverVolumeTopic::getEtalonJson()
{
    QJsonObject jsonMainObj({
                                { "Sender", "testSender" },
                                { "Value", 50 },
                                { "NeedToSave", true }
                            });

    return QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif
