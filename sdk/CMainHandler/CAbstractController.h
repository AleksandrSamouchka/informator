#pragma once

#include <QtCore/QObject>

#include <CSettingsHandler/CSettingsEntry/CSettingsEntry.h>
#include <CSettingsHandler/CSettingsHandler.h>
#ifdef USE_MQTT_HANDLER
    #include <CMqttHandler/CMqttClientHandler.h>
    #include <CMqttHandler/CMqttMessage.h>
    #include <CMqttTopics/CTopicValueEntry.h>
#endif

class QTimer;

class CAbstractController : public QObject
{
    Q_OBJECT

public:
    static constexpr auto timeoutWaitThreadQuits{ 99999 }; //! ~10 seconds

    explicit CAbstractController( QObject * parent = nullptr );

protected:
#ifdef USE_MQTT_HANDLER
    inline bool publishToMqttTopic(const QString & topicName, 
                            const QJsonObject &obj,
                            bool retain = false/*,
                            quint8 qos = 0*/)
    {
//        return publishToMqttTopic(topicName, obj, false, 0);
        return publishMqttMsg(CMqttMessage(topicName, obj, retain/*, qos*/));
    }

    inline bool publishToMqttTopic(const QString & topicName, 
                            const QJsonDocument &data,
                          bool retain = false/*,
                          quint8 qos = 0*/)
    {
//        return publishToMqttTopic(topicName, data, false, 0);
        return publishMqttMsg(CMqttMessage(topicName, data.toJson(), retain/*, qos*/));        
    }

    inline bool publishToMqttTopic(const QString & topicName, 
                            const QByteArray &data,
                          bool retain = false/*,
                          quint8 qos = 0*/)
    {
//        return publishToMqttTopic(topicName, data, false, 0);
        return publishMqttMsg(CMqttMessage(topicName, data, retain/*, qos*/));
    }


    inline bool publishToMqttTopic(CTopicValueEntry::ETopicsNamesIndex topicIndex,
                            const QJsonDocument &data,
                          bool retain = false/*,
                          quint8 qos = 0*/)
    {
//        return publishToMqttTopic(topicIndex, data, false, 0);
        return publishMqttMsg(CMqttMessage(topicIndex, data.toJson(), retain/*, qos*/));
    }

    inline bool publishToMqttTopic(CTopicValueEntry::ETopicsNamesIndex topicIndex,
                            const QJsonObject &obj,
                          bool retain = false/*,
                          quint8 qos = 0*/)
    {
//        return publishToMqttTopic(topicIndex, obj, false, 0);
        return publishMqttMsg(CMqttMessage(topicIndex, obj, retain/*, qos*/));
    }

    inline bool publishToMqttTopic(CTopicValueEntry::ETopicsNamesIndex topicIndex,
                            const QByteArray &data,
                          bool retain = false/*,
                          quint8 qos = 0*/)
    {
//        return publishToMqttTopic(topicIndex, data, false, 0);
        return publishMqttMsg(CMqttMessage(topicIndex, data, retain/*, qos*/));
    }


#endif

protected slots:
#ifdef USE_MQTT_HANDLER
    bool publishMqttMsg( const CMqttMessage & message );
#endif


    bool subscribeToMqttTopic( const QString & topicName );
    bool unsubscribeFromMqttTopic( const QString & topicName );
#ifdef USE_MQTT_HANDLER
    bool subscribeToMqttTopic( CTopicValueEntry::ETopicsNamesIndex topicIndex );
    bool unsubscribeFromMqttTopic( CTopicValueEntry::ETopicsNamesIndex topicIndex );
#endif
    virtual void OnMqttMessageReceived( const QString & topic,
                                        const QJsonObject & json );
#ifdef USE_MQTT_HANDLER
    virtual void OnMqttMessageReceived( CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                        const QJsonObject & json );
#endif

    inline void setSettingsOwner(const char *owner)
    {
        m_settingsOwner = owner;
    }

    inline QString getSettingsOwner(void) const
    {
        return m_settingsOwner;
    }

    inline void setModuleName(const char *moduleName)
    {
        m_moduleName = moduleName;
    }

    inline QString getModuleName() const 
    {
        return m_moduleName;
    }

    virtual bool getDefaultSettings(QList<CSettingsEntry> &list);
    virtual void loadSettings(QList < CSettingsEntry > &list);
    inline QList < CSettingsEntry > getSettingsList() const
    {
        return CSettingsHandler::getInstance()->getAllSettings(true);
    }

    inline void setSettingsEntry(CSettingsEntry &entry) const
    {
        CSettingsHandler::getInstance()->setSetting( entry );
    }

    static QByteArray makeSimpleJson( const QVariant & value );

private:
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

public slots:
    void attach();
    void detach();

protected slots:
    virtual void mqttConnected();
    virtual void mqttDisconnected();
#ifdef USE_MQTT_HANDLER
    virtual void mqttMessageReceived( const CMqttMessage & message );
#endif

private:
#ifdef USE_MQTT_HANDLER
    CMqttClientHandler * m_mqttClient;
#endif
    QTimer * m_keepaliveTimer;
    QString m_moduleName;
    QString m_settingsOwner;
};
