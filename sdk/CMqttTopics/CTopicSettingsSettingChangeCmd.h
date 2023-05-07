#ifndef CTOPICSETTINGSSETTINGCHANGECMD_H
#define CTOPICSETTINGSSETTINGCHANGECMD_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicSettingsSettingChangeCmd
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicSettingsSettingChangeCmd : public CTopicValueEntry
{
public:
    QString m_module{};
    QString m_settingName{};

public:
    CTopicSettingsSettingChangeCmd( const QString &module = QString{},
                                    const QString &settingName = QString{} );
    QByteArray prepareData() const override;
    bool initFromData( const CTopicValueEntry &value ) override;
    virtual ~CTopicSettingsSettingChangeCmd() override = default;
};

#endif // CTOPICSETTINGSSETTINGCHANGECMD_H
