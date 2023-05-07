#ifndef CONVIFFOUNDCAMERASTOPIC_H
#define CONVIFFOUNDCAMERASTOPIC_H

#include <Mqtt/Topics/CGrOnvifTopic.h>

/** ***********************************************************************
 * @class CNavigationGnssStateTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class COnvifFoundCamerasTopic : public CGrOnvifTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    struct CCameraParams
    {
        static const QStringList STATUS_STRINGS;
        enum class EStatus
        {
            unknown = 0,
            unknownDevice,
            errors,
            invalidAuth,
            disconnected,
            connected,
        };

        CCameraParams() = default;
        static QString statusString( const EStatus status )
        {
            return STATUS_STRINGS.value( static_cast< int >( status ), "НЕИЗВЕСТНО" );
        }

        QString m_ip{};
        QString m_mac{};
        QString m_firstStream{};
        QString m_secondStream{};
        EStatus m_status{};
    };

public:
    COnvifFoundCamerasTopic();
    virtual ~COnvifFoundCamerasTopic() override;
    inline const QList < CCameraParams > & getCameraList() const
    {
        return m_cameras;
    }
    void clearCameraList();
    void appendCameraParams( const CCameraParams & value );
    void setCameraList( const QList < CCameraParams > & value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QList < CCameraParams > m_cameras{};
};

#endif // CONVIFFOUNDCAMERASTOPIC_H
