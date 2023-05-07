#ifndef CTOPICAUDIOTRACKTOPLAYSTATUS_H
#define CTOPICAUDIOTRACKTOPLAYSTATUS_H

/** ***********************************************************************
 * @name CTopicAudioTrackToPlayStatus
 * @brief Содержимое топика с командой для проигрывания файла
 * @author Мальнев А
 * @date 28-05-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicAudioTrackToPlayStatus
 ** ***********************************************************************/
class CTopicAudioTrackToPlayStatus : public CTopicValueEntry
{
public:
    enum class EStatuses
    {
        queued = 0,
        playing,
        finished,
    };

public:
    CTopicAudioTrackToPlayStatus( const QByteArray &json = QByteArray{} );
    virtual bool initFromData( const CTopicValueEntry &value ) override;
    QByteArray prepareData() const override;
    virtual ~CTopicAudioTrackToPlayStatus() override = default;

public:
    EStatuses m_status{ EStatuses::finished };
    QString m_uuid{};
};

#endif // CTOPICAUDIOTRACKTOPLAYSTATUS_H
