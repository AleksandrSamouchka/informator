#ifndef CTOPICINTEGRATIONINFORMATOR_H
#define CTOPICINTEGRATIONINFORMATOR_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicIntegrationInformator
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicIntegrationInformator : public CTopicValueEntry
{
public:
    QString currentStation{};
    QString nextStation{};
    QString additionalInf{};
    bool onStation{ false };
    QString routeNum{};
    QString routeName{};
    QString subrouteName{};

public:
    CTopicIntegrationInformator() = default;

    bool initFromData( const CTopicValueEntry & value );
    virtual QByteArray prepareData() const;
    virtual ~CTopicIntegrationInformator() = default;
};

#endif // CTOPICINTEGRATIONINFORMATOR_H
