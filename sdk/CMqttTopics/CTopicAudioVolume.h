#ifndef CTOPICAUDIOVOLUME_H
#define CTOPICAUDIOVOLUME_H

/** ***********************************************************************
 * @class CTopicAudioVolume
 * @brief Топик для задания громкости звука
 * @author Мальнев А
 * @date 06-03-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicAudioVolume
 ** ***********************************************************************/
class CTopicAudioVolume : public CTopicValueEntry
{
public:
    QString m_sender{};
    int m_value{ 0 };
    bool m_needToSave{ false };

public:
    CTopicAudioVolume( const QString &sender = QString{}, 
                        int value = 0, 
                        bool needToSave = false );
    bool initFromData( const CTopicValueEntry &value );
    QByteArray prepareData();
    virtual ~CTopicAudioVolume() = default;
};

#endif // CTOPICAUDIOVOLUME_H
