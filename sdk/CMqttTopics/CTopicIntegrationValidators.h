#ifndef CTOPICINTEGRATIONVALIDATORS_H
#define CTOPICINTEGRATIONVALIDATORS_H

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CInformatorSubrouteInfo.h>

/** ***********************************************************************
 * @class CTopicIntegrationValidators
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicIntegrationValidators : public CTopicValueEntry
{
public:
    QString m_date{};
    QString m_validatorId{};
    qint64  m_routeId{};
    QString m_exitId{};
    QString m_parkId{};
    QString m_driverId{};
    QString m_cardId{};
    QString m_ticketId{};
    QString m_applicationTypeId{};
    QString m_ticketTypeId{};

public:
    CTopicIntegrationValidators( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value );
    virtual ~CTopicIntegrationValidators() = default;
};

#endif // CTOPICINTEGRATIONVALIDATORS_H

