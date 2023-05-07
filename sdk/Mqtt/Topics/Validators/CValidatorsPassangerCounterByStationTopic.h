#ifndef CVALIDATORSPASSANGERCOUNTERBYSTATIONTOPIC_H
#define CVALIDATORSPASSANGERCOUNTERBYSTATIONTOPIC_H

#include <Mqtt/Topics/CGrValidatorsTopic.h>

/** ***********************************************************************
 * @class CValidatorsPassangerCounterByStationTopic
 * @brief класс, пассажиропоток и валидаторы (по остановкам)
 ** ***********************************************************************/
class CValidatorsPassangerCounterByStationTopic : public CGrValidatorsTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
    enum class EStates
    {
        doorOpened = 0,
        doorClosed,
        inMovement,
        uncknownRoute,
        stageChanged,
    };

    static const QStringList STATES;

    struct SPassangerCounterInfo
    {
        SPassangerCounterInfo() = default;
        SPassangerCounterInfo( const SPassangerCounterInfo & other ) = default;
        SPassangerCounterInfo( const QString & channelName,
                               const int comeInCounter,
                               const int comeOutCounter ) :
            m_channelName{ channelName },
            m_comeInCounter{ comeInCounter },
            m_comeOutCounter{ comeOutCounter }
        { }

        QString m_channelName{};
        int m_comeInCounter{ 0 };       /*!< Число вошедших пассажиров */
        int m_comeOutCounter{ 0 };      /*!< Число вышедших пассажиров */
    };

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CValidatorsPassangerCounterByStationTopic();
    explicit CValidatorsPassangerCounterByStationTopic( const QByteArray &data );

    inline const QString & getRouteName() const
    {
        return m_routeName;
    }
    inline const QString & getStationName() const
    {
        return m_stationName;
    }
    inline int getStationNum() const
    {
        return m_stationNum;
    }
    inline int getStationStopID() const
    {
        return m_stationStopID;
    }
    inline const QList < SPassangerCounterInfo > & getPassangerCounterList() const
    {
        return m_counters;
    }
    inline int getTransactionCounter() const
    {
        return m_transactionCounter;
    }
    inline int getTransactionCounterPrevStage() const
    {
        return m_transactionCounterPrevStage;
    }
    inline EStates getState() const
    {
        return m_state;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setRouteName( const QString & value );
    void setStationNum( const int value );
    void setStationName( const QString & value );
    void setStationStopID( int value );
    void clearPassangerCounterList();
    void appendPassangerCounter( const SPassangerCounterInfo & value );
    void setTransactionCounter( const int value );
    void setTransactionCounterPrevStage( const int value );
    void setState( const EStates value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_routeName{};
    int m_stationNum{ -1 };
    QString m_stationName{};
    int m_stationStopID{-1};
    QList < SPassangerCounterInfo > m_counters{};
    int m_transactionCounter{ -1 };
    EStates m_state{ EStates::uncknownRoute };
    int m_transactionCounterPrevStage{ -1 };
};

#endif // CVALIDATORSPASSANGERCOUNTERBYSTATIONTOPIC_H
