#ifndef CTOPICONVIFFOUNDCAMERAS_H
#define CTOPICONVIFFOUNDCAMERAS_H

/** ***********************************************************************
 * @class CTopicOnvifFoundCameras
 * @brief Содержимое топика с найденными камерами по ONVIF
 * @author Мальнев А
 * @date 29-04-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/COnvifCameraParams.h>

/** ***********************************************************************
 * @class CTopicOnvifFoundCameras
 ** ***********************************************************************/
class CTopicOnvifFoundCameras : public CTopicValueEntry
{
public:
    enum ECameraState
    {
        unknown = 0,
        unknownDevice,
        error,
        invalidAuth,
        disconnected,
        connected,
    };

public:
    QList < COnvifCameraParams > m_cameras{};

public:
    CTopicOnvifFoundCameras( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    bool initFromData( const QJsonObject &jsonMainObj );
    virtual QByteArray prepareData() const override;
    virtual ~CTopicOnvifFoundCameras() override = default;
};

#endif // CTOPICONVIFFOUNDCAMERAS_H
