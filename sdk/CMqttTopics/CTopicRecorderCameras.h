#ifndef CTOPICRECORDERCAMERAS_H
#define CTOPICRECORDERCAMERAS_H

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CCameraState.h>

/** ***********************************************************************
 * @class CTopicRecorderCameras
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicRecorderCameras : public CTopicValueEntry
{
public:
    QStringList m_cameraIps{};
    QList< CCameraState > m_camerasList = QList< CCameraState >{};

public:
    CTopicRecorderCameras( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicRecorderCameras() override = default;
};

#endif // CTOPICRECORDERCAMERAS_H
