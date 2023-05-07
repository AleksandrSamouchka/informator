#ifndef CRECORDERCAMERASTOPIC_H
#define CRECORDERCAMERASTOPIC_H

#include <Mqtt/Topics/CGrRecorderTopic.h>

/** ***********************************************************************
 * @class CCameraState
 * @brief структура, описывающая состояние камеры
 ** ***********************************************************************/
class SCameraState
{
public:
    enum class EStreamForPlaying
    {
        primary = 0,
        secondary,
    };

    enum class EActiveState
    {
        offline = 0,
        online,
        recording,
    };

public:
    EActiveState m_activeState{ EActiveState::offline };
    int m_cameraNumber{ 0 };
    EStreamForPlaying m_streamForPlaying{ EStreamForPlaying::secondary };

    SCameraState() = default;
    SCameraState( const SCameraState & other ) = default;

    static EActiveState getActiveState( const QString & str );
};

/** ***********************************************************************
 * @class CRecorderCamerasTopic
 ** ***********************************************************************/
class CRecorderCamerasTopic : public CGrRecorderTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    CRecorderCamerasTopic();
    ~CRecorderCamerasTopic() override;

    inline const QList< SCameraState > & getCamerasList() const
    {
        return m_camerasList;
    }

    void setCamerasList( const QList< SCameraState > & value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QList< SCameraState > m_camerasList{};

};

#endif // CRECORDERCAMERASTOPIC_H
