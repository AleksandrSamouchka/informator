#ifndef CTOPICAUDIOTRACKTOPLAY_H
#define CTOPICAUDIOTRACKTOPLAY_H

/** ***********************************************************************
 * @name CTopicAudioTrackToPlay
 * @brief Содержимое топика с командой для проигрывания файла
 * @author Мальнев А
 * @date 28-05-2019
 ** ***********************************************************************/
#include <CMqttTopics/CTopicValueEntry.h>
#include <Mqtt/Topics/Audio/CAudioTrackToTopic.h>

#include <QtCore/QUuid>

/** ***********************************************************************
 * @class CTopicAudioTrackToPlay
 ** ***********************************************************************/
class CTopicAudioTrackToPlay : public CTopicValueEntry
{
public:
/*    enum class EAction
    {
        start = 0,
        stop,
    };
*/

public:
    CTopicAudioTrackToPlay();
    CTopicAudioTrackToPlay(const QByteArray &json);
    CTopicAudioTrackToPlay(ETopicsNamesIndex topicIndex);

    virtual bool initFromData( const CTopicValueEntry &value ) override;
    QByteArray prepareData() const override;
    virtual ~CTopicAudioTrackToPlay() override = default;

public:
    QString m_sender{ QCoreApplication::applicationName() };
    CAudioTrackToTopic::EAction m_action{ CAudioTrackToTopic::EAction::stop };
    QString m_track;
    QString m_uuid{ QUuid::createUuid().toString() };
};

#endif // CTOPICAUDIOTRACKTOPLAY_H
