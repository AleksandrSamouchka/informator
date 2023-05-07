#ifndef TEST_CCOREAPPLICATION2_H
#define TEST_CCOREAPPLICATION2_H

#include <QObject>
#include <QString>
#include <QDebug>

#include <CMainHandler/CCoreApplication.h>
#include <Mqtt/CMqttTopic.h>

#include <TestUnits/src/results/CTopicTestResult.h>

/** **********************************************************
 * @class test_CCoreApplication2
 * @brief Используется для тестирования класса CCoreApplication.
 * Подписывается на топик, принимает сообщения и
 * сохраняет их параметры
 * @author Пищулина И
 * **********************************************************/
class test_CCoreApplication2 : public CCoreApplication
{
    Q_OBJECT
public:
    test_CCoreApplication2( const QString & topicName,
                            QObject * parent = nullptr );
    virtual ~test_CCoreApplication2();

    inline int getNumbOfMessages() const
    {
        return m_numbOfMessages;
    }

    inline int getMaxDelay() const
    {
        return  m_maxDelay;
    }

    inline int getNumbMessageMaxDelay() const
    {
        return m_numbMessageMaxDelay;
    }

    void calcDelays();

public slots:
    void stopTest();

private:
/** overrided methods   ********************************/
    virtual int onAttach() override;
    virtual int onDetach() override;
/** ****************************************************/

/** *****************************************************
 * @fn subscribeToTopic
 * @brief Подписывается на топик
 * @param none
 ******************************************************/
    void subscribeToTopic();

/** ****************************************************
 * @fn subscribeToTopic
 * @brief Отписывается от топика
 * @param none
 *******************************************************/
    void unsubscribeFromTopic();

private slots:
/** *****************************************************
 * @fn topicHandler
 * @brief Обработка принятых сообщений
 * @param none
 ********************************************************/
    void topicHandler();

signals:
/** ******************************************************
 * @fn startTest
 * @brief Сигнал для начала публикации сообщений
 * @param none
 *******************************************************/
    void startTest();

private:
    CMqttTopic * m_mqttTopic{ nullptr };
    QMetaObject::Connection m_connectSubscribe;

    qint64 m_publishTs{}, m_receiveTs{};
    int m_numbMessage{ -1 };
    int m_numbOfMessages{ 0 };
    int m_numbMessageMaxDelay{};
    int m_maxDelay{};

/** @brief Сохранение результатов тестирования
 *
 */
    CTopicTestResult * m_testResults{};
};

#endif // TEST_CCOREAPPLICATION2_H
