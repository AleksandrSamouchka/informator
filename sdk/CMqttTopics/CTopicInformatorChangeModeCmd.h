#ifndef CTOPICINFORMATORCHANGEMODECMD_H
#define CTOPICINFORMATORCHANGEMODECMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicInformatorSelectedBusCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicInformatorChangeModeCmd : public CTopicValueEntry
{
public:
    enum class EModes
    {
        Mode_auto = 0,
        Mode_manual,
        Mode_unknown,
    };

public:
    EModes m_mode = EModes::Mode_auto;

public:
    CTopicInformatorChangeModeCmd( const EModes mode );
    QByteArray prepareData();
    virtual ~CTopicInformatorChangeModeCmd() = default;
};

#endif // CTOPICINFORMATORCHANGEMODECMD_H
