#ifndef TEST_CCOREAPPLICATIONPUBLISHER3_H
#define TEST_CCOREAPPLICATIONPUBLISHER3_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

#include <CMqttHandler/CMqttClientHandler.h>
#include <CMqttHandler/CMqttMessage.h>

class QTimer;

/** **********************************************************
 * @class test_CCoreApplicationPublisher3
 * @brief Используется для тестирования класса CCoreApplication.
 * Публикует сообщения в топики с заданным интервалом
 * @author Пищулина И
 * **********************************************************/

class test_CCoreApplicationPublisher3 : public QObject
{
    Q_OBJECT
public:
    test_CCoreApplicationPublisher3( QStringList & topicsNames,
                                     QObject * parent = nullptr );
    virtual ~test_CCoreApplicationPublisher3();

public slots:
    void startTest();

private slots:
/** ***************************************************************
 * @fn publishMqttMessage
 * @brief Публикация сообщения в топики
 * @param none
 * *****************************************************************/
    void publishMqttMessage();
    void stopPublish();

signals:
    void stopTest();

private:
    CMqttClientHandler * m_clientHandler{ nullptr };
    QStringList m_topicsNames{};
};

#endif // TEST_CCOREAPPLICATIONPUBLISHER3_H
