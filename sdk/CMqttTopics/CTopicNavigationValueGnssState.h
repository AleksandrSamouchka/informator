#ifndef CTOPICNAVIGATIONVALUEGNSSSTATE_H
#define CTOPICNAVIGATIONVALUEGNSSSTATE_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicNavigationValueGnssState
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicNavigationValueGnssState : public CTopicValueEntry
{
public:
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

public:
    CTopicNavigationValueGnssState( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual QByteArray prepareData() const override;

    virtual ~CTopicNavigationValueGnssState() override = default;
};

#endif // CTOPICNAVIGATIONVALUEGNSSSTATE_H
