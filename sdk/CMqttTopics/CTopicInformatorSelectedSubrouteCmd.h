#ifndef CTOPICINFORMATORSELECTEDSUBROUTECMD_H
#define CTOPICINFORMATORSELECTEDSUBROUTECMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicInformatorSelectedSubrouteCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicInformatorSelectedSubrouteCmd : public CTopicValueEntry
{
public:
    int m_subrouteNum;

public:
    CTopicInformatorSelectedSubrouteCmd();
    CTopicInformatorSelectedSubrouteCmd( int subroute );

    virtual bool initFromData( const CTopicValueEntry & value ) override;
    virtual QByteArray prepareData() const override;
    virtual ~CTopicInformatorSelectedSubrouteCmd() override = default;
};

#endif // CTOPICINFORMATORSELECTEDSUBROUTECMD_H
