#ifndef __SDKTESTUNITS_HPP__
#define __SDKTESTUNITS_HPP__

#include <QObject>
#include <QMetaMethod>
#include <QMetaObject>

#include <Mqtt/CMqttTopic.h>


//#define MNT_TEST
class SdkTestUnits : public QObject
{
    Q_OBJECT
    private slots:
        void initTestCase();
        void cleanupTestCase();
        void testCMainHandler();
        void testCAbstractController();


/** **********************************************************
 * @fn testCCoreApplication
 * @brief Тестируется класс CCoreApplication. Задает
 * интервал для публикаций и их количество
 * **********************************************************/
        //void testCCoreApplication();

///** **********************************************************
// * @fn testCCoreApplication1
// * @brief Тестируется класс CCoreApplication. Запускает
// * первую часть тестов
// * **********************************************************/
//        void testCCoreApplication1();

///** **********************************************************
// * @fn testCCoreApplication1
// * @brief Тестируется класс CCoreApplication. Запускает
// * вторую часть тестов
// * **********************************************************/
//        void testCCoreApplication2();


/** **********************************************************
 * @fn getTopicsList
 * @brief Регистрирует классы топиков, создает список топиков
 * для тестирования
 *  *********************************************************/
        QList <const QMetaObject *> getTopicsListForTest();

/** **********************************************************
 * @fn getTopicsObjects
 * @brief Создание объектов топиков
 * ***********************************************************/
        QList < CMqttTopic * > getTopicsObjects();

/** **********************************************************
 * @fn testTopicsEtalonJson
 * @brief Тестируются классы топиков (initFromData, parseData, prepareData). С помощью метода getEtalonJson
 * от каждого топика получаем json, который должен получиться на выходе.
 * Вызываем initFromData. Сравниваем эталонный json с полученным
 * ************************************************************/
        void testTopicsEtalonJson();

        //временный вывод
  //      void printDebugInfo( int period, int repetitions );

///** **********************************************************
// * @fn testCCoreApplicationOneIteration
// * @brief Запускает все тесты, где подписка на 1 топик
// * **********************************************************/
//        void testCCoreApplicationOneIterationOneTopic( int period, int repetitions );
///** **********************************************************
// * @fn testCCoreApplicationOneIteration
// * @brief Запускает все тесты, где подписка на много топиков
// * **********************************************************/
//        void testCCoreApplicationOneIterationManyTopics(int numbOfTopics );


///** **********************************************************
// * @fn testCCoreApplicationManyRepetitions
// * @brief (1) Тестируется subscribe, unsubscribe и прием сообщений.
// * После каждой публикации ПО подписывается на топик, принимает
// * сообщение, проверяет и отписывается. Много повторов
// * **********************************************************/
//        void testCCoreApplicationManyRepetitions( int period, int repetitions );

///** **********************************************************
// * @fn testCCoreApplicationManyPublications
// * @brief (2) Тестируется прием сообщений. ПО подписывается на один
// * топик и ждет публикаций. Затем ВО публикует много сообщений.
// * Затем проверяется, что они все пришли
// * **********************************************************/
//        void testCCoreApplicationManyPublications( int period, int repetitions );

///** **********************************************************
// * @fn testCCoreApplicationManyPublications2
// * @brief (4) Тестируется публикация сообщений. ВО подписывается
// * топик, ПО публикует сообщения. В конце проверяется
// * количество принятых сообщений
// * **********************************************************/
//        void testCCoreApplicationManyPublications2( int period, int repetitions );

///** **********************************************************
// * @fn testCCoreApplicationManyTopics
// * @brief (3) Тестируется прием сообщений. Подписывается на множество
// * топиков и ждет
// * **********************************************************/
//        void testCCoreApplicationManyTopics( int numbOfTopics );

///** **********************************************************
// * @fn testCCoreApplicationManyTopics2
// * @brief (5) Тестируется публикация сообщений. ВО подписывается на
// * много топиков, ПО публикует сообщениe
// * **********************************************************/
//        void testCCoreApplicationManyTopics2( int numbOfTopics );
private:
        bool m_addClassesFlag{ false };

};

#endif
