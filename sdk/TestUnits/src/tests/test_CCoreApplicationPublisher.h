#ifndef TEST_CCOREAPPLICATIONPUBLISHER_H
#define TEST_CCOREAPPLICATIONPUBLISHER_H

/** **********************************************************
 * @class test_CCoreApplicationPublisher
 * @brief Используется для тестирования класса CCoreApplication.
 * Публикует сообщения в топик
 * @author Пищулина И
 * **********************************************************/

#include <QObject>
#include <QHash>
#include <QMutex>
#include <QTimer>

#include <CSettingsHandler/CSettingsEntry/CSettingsEntry.h>
#include <CSettingsHandler/CSettingsHandler.h>
#include <CConsoleLogger/CConsoleLogger.h>

#ifdef USE_MQTT_HANDLER
#include <CMqttHandler/CMqttClientHandler.h>
#include <CMqttHandler/CMqttMessage.h>
#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/CMqttTopicConnection.h>
#endif

class QTimer;

/** **********************************************************
 * @class test_CCoreApplicationPublisher
 * **********************************************************/
class test_CCoreApplicationPublisher : public QObject
{
    Q_OBJECT
public:
    test_CCoreApplicationPublisher( const QString & topic,
                                    QObject * parent = nullptr );
    virtual ~test_CCoreApplicationPublisher();

public slots:
/** **********************************************************
 * @fn startTestRetain
 * @brief Начать тест проверку ретейн сообщений
 * **********************************************************/
    void startTestRetain( const int period, const int repetitions );
    void endIteration();

private slots:
    void publishMqttMessage();

signals:
    void messagePublished( int );
/** ***********************************************************/
/** **********************************************************
* @fn stopTestRetain
* @brief Остановить тест-проверку ретейн сообщений
* **********************************************************/
    void stopTestRetain();

private:
    CMqttClientHandler * m_ptr{ nullptr };
    QTimer * m_periodTimer{};

    QString m_topic{};
    int m_period{};
    int m_repetitions{};
    int m_numbMessage{ -1 };

};

#endif // TEST_CCOREAPPLICATIONPUBLISHER_H
