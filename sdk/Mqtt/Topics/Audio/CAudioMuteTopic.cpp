#include "CAudioMuteTopic.h"                           


CAudioMuteTopic::CAudioMuteTopic(const char *subTopicName) :
    CAudioTopic(subTopicName)
{
}

CAudioMuteTopic::~CAudioMuteTopic()
{
}

/*
class CAudioMuteDriverTopic : public CAudioMuteTopic
{
    Q_OBJECT
    
    public:
        CAudioMuteDriverTopic();
        virtual ~CAudioMuteDriverTopic();
};

class CAudioMuteSalonTopic : public CAudioMuteTopic
{
    Q_OBJECT

    public:
        CAudioMuteSalonTopic();
        virtual ~CAudioMuteSalonTopic();
};

class CAudioMuteSpeakingCityTopic : public CAudioMuteTopic
{
    Q_OBJECT

    public:
        CAudioMuteSpeakingCityTopic();
        virtual ~CAudioMuteSpeakingCityTopic();
};

                                                               
CAudioMuteDriverTopic::CAudioMuteDriverTopic() :
    CAudioTopic("MUTE_DRIVER")
{
}

CAudioMuteDriverTopic::~CAudioMuteDriverTopic()
{                                                              
}
*/
