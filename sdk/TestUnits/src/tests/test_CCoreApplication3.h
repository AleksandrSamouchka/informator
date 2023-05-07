#ifndef TEST_CCOREAPPLICATION3_H
#define TEST_CCOREAPPLICATION3_H

#include <QObject>
#include <QString>
#include <QDebug>

#include <CMainHandler/CCoreApplication.h>
#include <TestUnits/src/topics/CTestTopic.h>
#include <TestUnits/src/results/CTopicTestResult.h>

/** **********************************************************
 * @class test_CCoreApplication3
 * @brief Используется для тестирования класса CCoreApplication.
 * Подписывается сразу на заданные топики и ждет
 * сообщений. После отписывается
 * @author Пищулина И
 * **********************************************************/
class test_CCoreApplication3 : public CCoreApplication
{
    Q_OBJECT
public:
    test_CCoreApplication3( const QList < CTestTopic > & topics,
                            QObject * parent = nullptr );
    virtual ~test_CCoreApplication3();

    inline int getNumbOfMissedMessages() const
    {
        return m_numbOfMissedMessages;
    }

    inline QString getTopicNameMaxDelay() const
    {
        return m_topicMaxDelay;
    }

    inline int getMaxDelay() const
    {
        return m_maxDelay;
    }

    void calcMaxDelay();

public slots:
    void stopTest();

signals:
/** ******************************************************
* @fn startTest
* @brief Сигнал для начала публикации сообщений
* @param none
*******************************************************/
    void startTest();

private:
/** overrided methods   ********************************/
    virtual int onAttach() override;
    virtual int onDetach() override;
/** ****************************************************/

/** *****************************************************
* @fn subscribeToTopic
* @brief Подписывается на топики
* @param none
******************************************************/
    void subscribeToTopics();

/** ****************************************************
* @fn subscribeToTopic
* @brief Отписывается от топиков
* @param none
*******************************************************/
    void unsubscribeFromTopics();

private slots:
/** *****************************************************
* @fn topicHandler
* @brief Обработка принятых сообщений
* @param none
********************************************************/
    void topicHandler();

private:
    QList < CTestTopic > m_mqttTopics{};
    QList < CTopicTestResult> m_topicTestResults{};

    QString m_topicMaxDelay{};
    int m_maxDelay{};

    qint64 m_publishTs{}, m_receiveTs{};
    int m_numbMessage{};

    int m_numbOfMissedMessages{};

};

#endif // TEST_CCOREAPPLICATION3_H

