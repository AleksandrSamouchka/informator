#ifndef CSETTINGSSETTINGCHANGETOPIC_H
#define CSETTINGSSETTINGCHANGETOPIC_H

#include <Mqtt/Topics/CGrSettingsTopic.h>

/** ***********************************************************************
 * @class CSettingsSettingChangeTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CSettingsSettingChangeTopic : public CGrSettingsTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif
public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CSettingsSettingChangeTopic( const QString & module = QString{},
                                          const QString & settingName = QString{} );
    explicit CSettingsSettingChangeTopic( const QByteArray &data );

    inline const QString & module() const
    {
        return m_module;
    }
    inline const QString & settingName() const
    {
        return m_settingName;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setModule( const QString & module );
    void setSettingName( const QString & settingName );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_module{};
    QString m_settingName{};
};

#endif // CSETTINGSSETTINGCHANGETOPIC_H
