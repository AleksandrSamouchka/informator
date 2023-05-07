#ifndef CTOPICINFORMATORSELECTEDMODE_H
#define CTOPICINFORMATORSELECTEDMODE_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicInformatorSelectedMode
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicInformatorSelectedMode : public CTopicValueEntry
{
public:
    enum class EModes
    {
        Mode_auto = 0,
        Mode_manual,
        Mode_unknown,
    };

public:
    EModes m_mode = EModes::Mode_unknown;

public:
    CTopicInformatorSelectedMode( const QByteArray &json );
    bool initFromData();
    virtual ~CTopicInformatorSelectedMode() = default;
};

#endif // CTOPICINFORMATORSELECTEDMODE_H
