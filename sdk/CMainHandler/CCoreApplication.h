#ifndef CCOREAPPLICATION_H
#define CCOREAPPLICATION_H

#include <CSettingsHandler/CSettingsEntry/CSettingsEntry.h>
#include <CSettingsHandler/CSettingsHandler.h>
#include <CConsoleLogger/CConsoleLogger.h>

#ifdef USE_MQTT_HANDLER
#include <CMqttHandler/CMqttClientHandler.h>
#include <CMqttHandler/CMqttMessage.h>
#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CMqttTopicConnection.h>
#include <Mqtt/CMqttTopic.h>
#endif

#include <QtCore/QReadWriteLock>
#include <QtCore/QMultiHash>
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QTimer>

class CCoreApplication : public QObject
{
    Q_OBJECT

public:
    explicit CCoreApplication( QObject * parent = nullptr );

protected slots:
#ifdef USE_MQTT_HANDLER
    bool publishToMqttTopic(const CMqttTopic * topic,
                            bool retain = false);
    bool publishToMqttTopic(const CMqttTopic & topic,
                            bool retain = false);

    bool publishToMqttTopic(const QString &connName,
                            const CMqttTopic * topic,
                            bool retain = false);

#pragma message "Deprecated, use CMqttTopic classes instead"
    bool publishToMqttTopic(const QString & topicName,
                                              const QJsonObject &obj,
                                              bool retain = false);
/*#pragma message "Deprecated, use CMqttTopic classes instead"
    bool publishToMqttTopic(const QString & topicName,
                                              const QJsonDocument &data,
                                              bool retain = false);
*/
#pragma message "Deprecated, use CMqttTopic classes instead"
    bool publishToMqttTopic(const QString & topicName,
                                              const QByteArray &data,
                                              bool retain = false);

#pragma message "Deprecated, use CMqttTopic classes instead"
    bool publishToMqttTopic( CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                               const QJsonObject &obj,
                                               bool retain = false);
#pragma message "Deprecated, use CMqttTopic classes instead"
    bool publishToMqttTopic( CTopicValueEntry::ETopicsNamesIndex topicIndex,
                                               const QByteArray &data,
                                               bool retain = false);

#endif

protected slots:
#ifdef USE_MQTT_HANDLER
#pragma message "Deprecated, use CMqttTopic classes instead"
    bool publishMqttMsg( const QString &connName, const CMqttMessage & message );
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

    virtual bool getDefaultAppSettings(QList<CSettingsEntry> &list);
    virtual void applyAppSettings(const QList < CSettingsEntry > &list);

    inline QList < CSettingsEntry > getSettingsList() const
    {
        return CSettingsHandler::getInstance()->getAllSettings(true);
    }
    inline void setSettingsEntry(CSettingsEntry &entry) const
    {
        CSettingsHandler::getInstance()->setSetting( entry );
    }
//    static QByteArray makeSimpleJson( const QVariant & value );

protected:
    virtual int onAttach() = 0;
    virtual int onDetach() = 0;

    bool loadSystemSettings();

    inline const char *getLocalMqttConnectionName() const
    {
        return "local";
    }

#ifdef USE_MQTT_HANDLER
    bool subscribeToMqttTopic( const QString &connName,
                               CMqttTopic * topic );
    bool subscribeToMqttTopic( CMqttTopic * topic );

    bool unsubscribeFromMqttTopic( const QString &connName,
                                   CMqttTopic * topic );
    bool unsubscribeFromMqttTopic( CMqttTopic * topic );

    CMqttClientHandler *createMqttConnection(const QString &connName,
                                             const QString &host,
                                             quint16 port);

    CMqttClientHandler *createMqttConnection2(const QString &connName,
                                              const QString &host,
                                              quint16 port,
                                              const QString &user,
                                              const QString &password);

private slots:
    bool subscribeToMqttTopic(CMqttClientHandler *conn, const QString &topicName);
    void processReceivedMqttMessage( const CMqttMessage & message );
#endif

protected:
#ifdef USE_MQTT_HANDLER

/** ***********************************************************************
 * @fn getMqttConnection
 * @brief Возвращает указатель на объект mqtt коннекта,
 * если объект с указанным именем не найден - возвращает nullptr
 * @param const QString & connName
 * @return CMqttClientHandler *
 ** ***********************************************************************/
    CMqttClientHandler *getMqttConnection(const QString &connName);
/** ***********************************************************************
 * @fn deleteMqttConnection
 * @brief Удаляет mqtt клиента из списка с указанным именем (если найден).
 * Если установлен флаг deleteConnection - объект (если найден) закрывает
 * соединение и удаляется полностью
 * @param const QString & connName
 * @param const bool deleteConnection = false
 ** ***********************************************************************/
    void deleteMqttConnection( const QString &connName,
                               const bool deleteConnection = false );

    virtual void onMqttMessageReceived(const QString& topicName, const QByteArray& payload);

#pragma message "Deprecated, use CMqttTopic classes instead"
    template <typename Func>
    inline bool subscribeToMqttTopic(const QString &connName,
                                     const QString &topicName,
                                     typename QtPrivate::FunctionPointer<Func>::Object *receiver,
                                     Func slot)
    {
        CMqttClientHandler *conn = getMqttConnection(connName);
        if(conn)
        {
            if(subscribeToMqttTopic(conn, topicName))
            {
                connect(conn, &CMqttClientHandler::messageReceived, [=](const CMqttMessage &message) {
                    if(message.getTopicName() == topicName)
                    {
                        CTopicValueEntry    topicValue(message.getMessage(), message.getTopicName());
                        (receiver->*slot)(topicValue);
                    }
                });
                return true;
            }
        }
        return false;
    }

#pragma message "Deprecated, use CMqttTopic classes instead"
    template <typename Func>
    bool subscribeToMqttTopic(const QString &connName,
                              CTopicValueEntry::ETopicsNamesIndex topicIndex,
                              typename QtPrivate::FunctionPointer<Func>::Object *receiver,
                              Func slot)
    {
        QString topicName = CTopicValueEntry::getTopicName(topicIndex);

        if(topicName.isEmpty())
        {
            PRINT_CONSOLE_MESSAGE_ERROR(QString("Unable to get topic name for index %1").
                                        arg(static_cast<int>(topicIndex)));

            return false;
        }

        return subscribeToMqttTopic(connName, topicName, receiver, slot);
    }
#pragma message "Deprecated, use CMqttTopic classes instead"
    template <typename Func>
    bool subscribeToMqttTopic(CTopicValueEntry::ETopicsNamesIndex topicIndex,
                              typename QtPrivate::FunctionPointer<Func>::Object *receiver,
                              Func slot)
    {
        return subscribeToMqttTopic(getLocalMqttConnectionName(), topicIndex, receiver, slot);
    }

    template <typename Func>
    bool subscribeToMqttTopic(const QString &topicName,
                              typename QtPrivate::FunctionPointer<Func>::Object *receiver,
                              Func slot)
    {
        return subscribeToMqttTopic(getLocalMqttConnectionName(), topicName, receiver, slot);
    }


    bool unsubscribeFromMqttTopic(const QString &topicName);
    bool unsubscribeFromMqttTopic(const QString &connName, const QString &topicName);
#endif
    const QString getSystemSetting(const QString &keyName) const;

public slots:
    int attach();
    int detach();
    virtual void onSubscribeToTopic(CMqttTopic* topic);
    virtual void onSubscribeToTopicByName(const QString& topic);

private:
#ifdef USE_MQTT_HANDLER
    // connName, connPointer
    QHash<QString, QPointer<CMqttClientHandler> > m_mqttConnections{};
    QMutex m_mqttConnectionsMutex{};
    // topicName, topicPointer
    QMultiHash<QString, QPointer<CMqttTopic> > m_mqttTopics{};
    QReadWriteLock m_mqttTopicsMutex{};

#endif
    QHash<QString, QString> m_systemSettings{};
    QTimer * m_keepaliveTimer{};
    QString m_moduleName{};
    QString m_settingsOwner{};
    bool m_connectionStatus{ false };
};

#endif
