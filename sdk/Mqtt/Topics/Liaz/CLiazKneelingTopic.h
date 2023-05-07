#ifndef CLIAZKNEELINGTOPIC_H
#define CLIAZKNEELINGTOPIC_H

#include "Mqtt/Topics/CGrLiazTopic.h"

/** ***********************************************************************
 * @class CLiazKneelingTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CLiazKneelingTopic : public CGrLiazTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

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
    CLiazKneelingTopic();
    explicit CLiazKneelingTopic( const EKneelingStates state );
    inline EKneelingStates getState() const
    {
        return m_state;
    }

public:
    void setState( const EKneelingStates value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    EKneelingStates m_state{ EKneelingStates::Kneeling_unknown };
};

#endif // CLIAZKNEELINGTOPIC_H
