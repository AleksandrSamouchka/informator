#ifndef CINFORMATORROUTESTATETOPIC_H
#define CINFORMATORROUTESTATETOPIC_H

#include <Mqtt/Topics/CGrInformatorTopic.h>


/** ***********************************************************************
 * @class CInformatorRouteStateTopic
 * @brief класс, умеющий работать с данными в топике
 ** ***********************************************************************/
class CInformatorRouteStateTopic : public CGrInformatorTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
/** ***********************************************************************
 * @struct CInformatorStationInfo
 * @brief класс, содержащий информацию об остановочном пункте
 ** ***********************************************************************/
    struct CInformatorStationInfo
    {
        public:
            enum EStationType
            {
                Station_start = 0,
                Station_intermidiate,
                Station_end,
            };

        public:
            int m_number{ 0 };
            QString m_name{};
            qint64 m_arriveTime{ 0 };
            qint64 m_duration{ 0 };
            double m_latitude{ 0 };     /*!< NOT USED */
            double m_longitude{ 0 };    /*!< NOT USED */
            int m_type{ 0 };            /*!< NOT USED */
            QString m_additionalInfo{ "" };

        public:
            CInformatorStationInfo() = default;
            CInformatorStationInfo( const CInformatorStationInfo & other ) = default;
            explicit CInformatorStationInfo( int number,
                                             const QString &name,
                                             qint64 arriveTime,
                                             qint64 duration,
                                             double latitude = 0.0,
                                             double longitude = 0.0,
                                             int type = 0,
                                             QString additionalInfo = "") :
                m_number{ number },
                m_name( name ),
                m_arriveTime{ arriveTime },
                m_duration{ duration },
                m_latitude{ latitude },
                m_longitude{ longitude },
                m_type{ type },
                m_additionalInfo{ additionalInfo }
            { }
            ~CInformatorStationInfo() = default;
    };

/** ***********************************************************************
 * @struct CInformatorStationInfo
 * @brief класс, содержащий информацию об остановочных пунктах подмаршрута
 ** ***********************************************************************/
    struct CInformatorSubrouteInfo
    {
    public:
        int m_number{ 0 };
        QString m_name{};
        QList < CInformatorStationInfo > m_stations{};

    public:
        CInformatorSubrouteInfo() = default;
        CInformatorSubrouteInfo( const CInformatorSubrouteInfo & other ) = default;
        explicit CInformatorSubrouteInfo( int number,
                                          const QString &name,
                                          const QList < CInformatorStationInfo > &stations = QList < CInformatorStationInfo >{} ) :
            m_number{ number },
            m_name( name ),
            m_stations( stations )
        { }
        ~CInformatorSubrouteInfo() = default;
    };

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CInformatorRouteStateTopic();
    explicit CInformatorRouteStateTopic( const QByteArray &data );

#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE virtual QByteArray getEtalonJson() const;
#endif

    inline qint64 getRouteId() const
    {
        return m_routeId;
    }
    inline qint64 getTripId() const
    {
        return m_tripId;
    }
    inline const QString & getRouteNum() const
    {
        return m_routeNum;
    }
    inline int getCurrentSubroute() const
    {
        return m_currentSubroute;
    }
    inline int getUserCurrentStation() const
    {
        return m_userCurrentStation;
    }
    inline int getCurrentStation() const
    {
        return m_currentStation;
    }
    inline int getNextStation() const
    {
        return m_nextStation;
    }
    inline int getUserNextStation() const
    {
        return m_userNextStation;
    }
    inline int getSubroutePercentage() const
    {
        return m_subroutePercentage;
    }
    inline bool getIsLocateOnStation() const
    {
        return m_isLocateOnStation;
    }
    inline const QList < CInformatorSubrouteInfo > & getSubroutes() const
    {
        return m_subroutes;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setRouteNum( const QString & value );
    void setCurrentRoute(const int value);
    void setRouteId(const qint64 value);
    void setTripId(const qint64 value);
    void setCurrentSubroute( const int value );
    void setUserCurrentStation( const int value );
    void setCurrentStation( const int value );
    void setNextStation( const int value );
    void setUserNextStation( const int value );
    void setSubroutePercentage( const int value );
    void setIsLocateOnStation( const bool value );

    void clearSubroutes();
    void appendSubroute( const CInformatorSubrouteInfo & value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    qint64 m_routeId{ -1 };
    qint64 m_tripId{ -1 };
    QString m_routeNum;
    int m_currentRoute{ 0 };
    int m_currentSubroute{ 0 };
    int m_userCurrentStation{ 0 };
    int m_currentStation{ 0 };
    int m_nextStation{ 0 };
    int m_userNextStation{ 0 };
    int m_subroutePercentage{ 0 };
    bool m_isLocateOnStation{ false };

    QList < CInformatorSubrouteInfo > m_subroutes{};
};

#endif // CINFORMATORROUTESTATETOPIC_H
