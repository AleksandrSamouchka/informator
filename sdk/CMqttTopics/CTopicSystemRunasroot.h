#ifndef CTOPICSYSTEMRUNASROOT_H
#define CTOPICSYSTEMRUNASROOT_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicSystemRunasroot
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicSystemRunasroot : public CTopicValueEntry
{
public:
    CTopicSystemRunasroot( const QByteArray &json = QByteArray{} );
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicSystemRunasroot() override = default;

public:
    QString mId{};
    QString mAction{};
    QString mStatus{};
};

#endif // CTOPICSYSTEMRUNASROOT_H
