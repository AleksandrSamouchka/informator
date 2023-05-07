#ifndef CTOPICLIAZKNEELING_H
#define CTOPICLIAZKNEELING_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicLiazKneeling
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicLiazKneeling : public CTopicValueEntry
{
public:
    enum EKneelingStates
    {
        Kneeling_notActive = 0,
        Kneeling_loweringActive,
        Kneeling_kneelingLevelReached,
        Kneeling_liftingActive,
        Kneeling_kneelingAborted,
        Kneeling_error,

        Kneeling_unknown,
    };

public:
    EKneelingStates m_state{ EKneelingStates::Kneeling_unknown };

public:
    CTopicLiazKneeling( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicLiazKneeling() override = default;
};


#endif // CTOPICLIAZKNEELING_H
