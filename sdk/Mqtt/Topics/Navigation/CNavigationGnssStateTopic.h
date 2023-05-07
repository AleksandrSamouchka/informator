#ifndef CNAVIGATIONGNSSSTATETOPIC_H
#define CNAVIGATIONGNSSSTATETOPIC_H

#include <Mqtt/Topics/CGrNavigationTopic.h>

/** ***********************************************************************
 * @class CNavigationGnssStateTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CNavigationGnssStateTopic : public CGrNavigationTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CNavigationGnssStateTopic( );
    explicit CNavigationGnssStateTopic( const QByteArray &data );

    virtual ~CNavigationGnssStateTopic() override;

    inline uint32_t time() const
    {
        return m_time;
    }
    inline bool isValid() const
    {
        return m_isValid;
    }
    inline const QString & fixType() const
    {
        return m_fixType;
    }
    inline double latitude() const
    {
        return m_latitude;
    }
    inline double longitude() const
    {
        return m_longitude;
    }
    inline double speed() const
    {
        return m_speed;
    }
    inline int altitude() const
    {
        return m_altitude;
    }
    inline int dir() const
    {
        return m_dir;
    }
    inline int satVisible() const
    {
        return m_satVisible;
    }
    inline int satUsed() const
    {
        return m_satUsed;
    }
    inline int hdop() const
    {
        return m_hdop;
    }
    inline int vdop() const
    {
        return m_vdop;
    }
    inline int pdop() const
    {
        return m_pdop;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setTime( const uint32_t value );
    void setIsValid( const bool value );
    void setFixType( const QString & value );
    void setLatitude( const double value );
    void setLongitude( const double value );
    void setSpeed( const double value );
    void setAltitude( const int value );
    void setDir( const int value );
    void setSatVisible( const int value );
    void setSatUsed( const int value );
    void setHDop( const int value );
    void setVDop( const int value );
    void setPDop( const int value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    uint32_t m_time = 0;
    bool m_isValid{ false };
    QString m_fixType = "2D";
    double m_latitude = 0.0;
    double m_longitude = 0.0;
    double m_speed = 0.0;
    int m_altitude = 0;
    int m_dir = 0;
    int m_satVisible = 0;
    int m_satUsed = 0;
    int m_hdop = 0;
    int m_vdop = 0;
    int m_pdop = 0;
};

#endif // CNAVIGATIONGNSSSTATETOPIC_H
