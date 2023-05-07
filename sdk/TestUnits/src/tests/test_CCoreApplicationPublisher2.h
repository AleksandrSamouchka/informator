#ifndef TEST_CCOREAPPLICATIONPUBLISHER2_H
#define TEST_CCOREAPPLICATIONPUBLISHER2_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

#include <CMqttHandler/CMqttClientHandler.h>
#include <CMqttHandler/CMqttMessage.h>

class QTimer;

/** **********************************************************
 * @class test_CCoreApplicationPublisher2
 * @brief Используется для тестирования класса CCoreApplication.
 * Публикует сообщения в топик с заданным интервалом
 * @author Пищулина И
 * **********************************************************/

class test_CCoreApplicationPublisher2 : public QObject
{
    Q_OBJECT
public:
    test_CCoreApplicationPublisher2( const QString & topicName,
                                     QObject * parent = nullptr );
    virtual ~test_CCoreApplicationPublisher2();

public slots:
    void startTest( const int period, const int repetitions );

private slots:
/** ***************************************************************
 * @fn publishMqttMessage
 * @brief Публикация сообщения в топик
 * @param none
 * *****************************************************************/
    void publishMqttMessage();

signals:
/** ***************************************************************
 * @fn stopTest
 * @brief Сигнал об окончании публикаций
 * @param none
 * *****************************************************************/
    void stopTest();

private:
    CMqttClientHandler * m_clientHandler{ nullptr };

    QTimer * m_periodTimer{};
    QString m_topicName{};
    int m_repetitions{};
    int m_period{};
    int m_numbMessage{ -1 };
};

#endif // TEST_CCOREAPPLICATIONPUBLISHER2_H
