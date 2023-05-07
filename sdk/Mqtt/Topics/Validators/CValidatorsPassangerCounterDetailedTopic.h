#ifndef CVALIDATORSPASSANGERCOUNTERDETAILEDTOPIC_H
#define CVALIDATORSPASSANGERCOUNTERDETAILEDTOPIC_H

#include <Mqtt/Topics/CGrValidatorsTopic.h>
#include <QDateTime>

/** ***********************************************************************
 * @class CValidatorsPassangerCounterDetailedTopic
 * @brief класс, пассажиропоток и валидаторы (по остановкам)
 ** ***********************************************************************/
class CValidatorsPassangerCounterDetailedTopic : public CGrValidatorsTopic
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
        unknownRoute,
        stageChanged,
        tripStart,
        tripEnd
    };
    static const QStringList STATES;

/** ***********************************************************************
 * @struct SPassangerCounterInfo
 ** ***********************************************************************/
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
/** ***********************************************************************
 * @struct SPassangerData
 ** ***********************************************************************/
    struct SPassangerData
    {
        SPassangerData() = default;
        SPassangerData( const SPassangerData & other ) = default;
        SPassangerData( const int stationNum,
                        const QString & stationName,
                        const int transactionCounter,
                        const QList < SPassangerCounterInfo > & counters ) :
            m_stationNum{ stationNum },
            m_stationName{ stationName },
            m_transactionCounter{ transactionCounter },
            m_counters( counters )
        { }

        int m_stationNum{ -1 };
        QString m_stationName{};
        int m_stationStopID{-1};
        int m_transactionCounter{ -1 };
        QList < SPassangerCounterInfo > m_counters{};
    };

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CValidatorsPassangerCounterDetailedTopic();
    explicit CValidatorsPassangerCounterDetailedTopic( const QByteArray &data );

    inline const QString & getRouteName() const
    {
        return m_routeName;
    }
    inline qint64 getRouteID() const
    {
        return m_routeID;
    }
    inline EStates getState() const
    {
        return m_state;
    }
    inline const SPassangerData & getCurrentData() const
    {
        return m_currentData;
    }
    inline const SPassangerData & getPreviousData() const
    {
        return m_previousData;
    }
    inline const QList < SPassangerCounterInfo > & getAbsolutPassangerCounters() const
    {
        return m_absolutCounters;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setRouteName( const QString & value );
    void setRouteID( qint64 value );
    void setState( const EStates value );
    void setCurrentData( const SPassangerData & value );
    void setPreviousData( const SPassangerData & value );
    void setAbsolutPassangerCounters( const QList < SPassangerCounterInfo > & value );
    void setAbsoluteTransactionCounter(int transactionCount);
protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_routeName{};
    qint64 m_routeID{-1};
    EStates m_state{ EStates::unknownRoute };

    SPassangerData m_currentData{};
    SPassangerData m_previousData{};


    QList < SPassangerCounterInfo > m_absolutCounters{};
    int                             m_absoluteTransactionCounter;
};

#endif // CVALIDATORSPASSANGERCOUNTERDETAILEDTOPIC_H
