#ifndef TEST_CCOREAPPLICATION_H
#define TEST_CCOREAPPLICATION_H

#include <QObject>
#include <CMainHandler/CCoreApplication.h>
#include <QString>
#include <Mqtt/CMqttTopic.h>

#include <TestUnits/src/results/CRetainTestResult.h>

class QTimer;


/** **********************************************************
 * @class test_CCoreApplication
 * @brief Используется для тестирования класса CCoreApplication.
 * Подписывается на топик после публикации, получает ретейн-сообщение
 * и отписывается. Собирает данные об интервалах между подпиской и приемом
 * @author Пищулина И
 * **********************************************************/

class test_CCoreApplication : public CCoreApplication
{
    Q_OBJECT
public:
    test_CCoreApplication( const QString &topicName,
                           QObject * parent = nullptr );
    virtual ~test_CCoreApplication();

    inline int getNumbOfMessages() const
    {
        return  m_numbOfMessages;
    }

    inline int getPubSubMaxDelay() const
    {
        return m_pubSubMaxDelay;
    }

    inline int getSubReceiveMaxDelay() const
    {
        return  m_subReceiveMaxDelay;
    }

    inline int getNumbMsgPubSubDelay() const
    {
        return m_numbMsgPubSubDelay;
    }

    inline int getNumbMsgSubReceiveDelay() const
    {
        return m_numbMsgSubReceiveDelay;
    }

    void calcDelays();

public slots:
    void messagePublishedHandler(int index);
    void stopTestRetain();

private:
/** overrided methods   ********************************/
    virtual int onAttach() override;
    virtual int onDetach() override;
/** ****************************************************/

/** **********************************************************
 * @fn unsubscribeFromTopic
 * @brief Отписка от топика
 * **********************************************************/
    void unsubscribeFromTopic();

protected:
/** **********************************************************
 * @fn subscribeToTopic
 * @brief Подписка на топик
 * **********************************************************/
    void subscribeToTopic();

signals:
    void startTestRetain();
    void endIteration();

private slots:
/** **********************************************************
 * @fn topicHandler
 * @brief Обработка пришедшего сообщения
 * **********************************************************/
    void topicHandler();

/** **********************************************************
 * @fn timeout
 * @brief Выход в случае, если сообщение не пришло в
 * установленный промежуток времени
 * **********************************************************/
    void timeout();

/** **********************************************************
 * @fn stopCurrentIteration
 * @brief Завершение ожидания сообщения
 * @param bool success - false при выходе по таймауту
 * **********************************************************/
    void stopCurrentIteration( bool success );

private:
    int m_currentIndex{ 0 };
    QMetaObject::Connection m_connectSubscribe;

    QTimer * m_timeoutTimer{};
    qint64 m_publishTs{}, m_subscribeTs{}, m_receiveTs{};

    CMqttTopic * m_mqttTopic{ nullptr };
    int m_numbOfMessages{ 0 };
    int m_numbMessage{};

    int m_numbMsgPubSubDelay{};
    int m_numbMsgSubReceiveDelay{};
    int m_subReceiveMaxDelay{};
    int m_pubSubMaxDelay{};

/** @brief Сохранение результатов тестирования
 * - успех / не успех
 * - время от подписки до получения
 */
    QList < CRetainTestResult > m_retainTestResults{} ;
};


#endif // TEST_CCOREAPPLICATION_H
