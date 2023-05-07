#ifndef CONVIFCAMERAPARAMS_H
#define CONVIFCAMERAPARAMS_H

/** ***********************************************************************
 * @class COnvifCameraParams
 * @brief Параметры камеры, обнаруженной по ONVIF
 * @author Мальнев А
 * @date 26-04-2019
 ** ***********************************************************************/

#include <QtCore/QObject>

/** ***********************************************************************
 * @class COnvifCameraParams
 ** ***********************************************************************/
class COnvifCameraParams
{
public:
    enum class EStatus
    {
        unknown = 0,
        unknownDevice,
        errors,
        invalidAuth,
        disconnected,
        connected,
    };

public:
    COnvifCameraParams() = default;
    static QString statusString( const EStatus status );


public:
    QString m_ip{};
    QString m_mac{};
    QString m_firstStream{};
    QString m_secondStream{};
    EStatus m_status{};
};


#endif // CONVIFCAMERAPARAMS_H
