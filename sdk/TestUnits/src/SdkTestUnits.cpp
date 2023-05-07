#include <QTest>
#include <QtCore/QCoreApplication>

#include <CMainHandler/CMainHandler.h>
#include <CConsoleLogger/CConsoleLogger.h>
#include <CMqttTopics/CCameraState.h>

#include "SdkTestUnits.hpp"

#include "tests/test_CAbstractController.hpp"

#include "tests/test_CCoreApplication.h"
#include "tests/test_CCoreApplicationPublisher.h"

#include "tests/test_CCoreApplication2.h"
#include "tests/test_CCoreApplicationPublisher2.h"

#include "tests/test_CCoreApplication3.h"
#include "tests/test_CCoreApplicationPublisher3.h"

#include "tests/test_CCoreApplication4.h"
#include "tests/test_CCoreApplicationSubscriber4.h"
#include "TestUnits/src/topics/CTestTopic.h"

#include "tests/test_CCoreApplication5.h"
#include "tests/test_CCoreApplicationSubscriber5.h"

#include "topics_list.h"


void SdkTestUnits::initTestCase()
{
qDebug() << "==========> Init";
}

void SdkTestUnits::cleanupTestCase()
{
qDebug() << "==========> Cleanup";
}


void SdkTestUnits::testCMainHandler()
{
    QCoreApplication::setApplicationVersion( "1.0.0" );
    {
        int argc = 2;
        char *argv[] = {(char*)"./test_app", (char*)"test_arg"};
        QCoreApplication app(argc, argv);
        QVERIFY(CMainHandler::parseArgs(false) == 0);
    }
    {
        int argc = 2;
        char *argv[] = {(char*)"./test_app", (char*)"sdfgsdfgsdfg"};
        QCoreApplication app(argc, argv);
        QVERIFY(CMainHandler::parseArgs(false) == -1);
    }
    {
        int argc = 2;
        char *argv[] = {(char*)"./test_app", (char *)"--quiet"};
        QCoreApplication app(argc, argv);
        QVERIFY(CMainHandler::parseArgs(false) == 0);
        test_CAbstractController    *t1 =  new test_CAbstractController(&app);
        QVERIFY(CMainHandler::runController( t1 ) == 0);
        delete t1;

        test_CAbstractController    *t2 = new test_CAbstractController(&app);
        t2->setExitCode(-1);
        QVERIFY(CMainHandler::runController( t2 ) == -1);
        delete t2;
    }
}

void SdkTestUnits::testCAbstractController()
{
    int argc = 2;
    char *argv[] = {(char*)"./test_app", (char *)"--quiet"};
    QCoreApplication app(argc, argv);
    test_CAbstractController    t(&app);

    CMainHandler::runController( &t );

    QVERIFY(t.test_getDefaultSettings() == true);
    QVERIFY(t.test_loadSettings() == true);
    QVERIFY(t.test_onAttach() == true);
    QVERIFY(t.test_onDetach() == true);
    QVERIFY(t.test_mqttConnected() == true);
}

//void SdkTestUnits::testCCoreApplication1()
//{
//    QVector<int> period{ 50, 100 };
//    int repetitions{ 100 };
//    for ( int i = 0; i < period.size(); i++ )
//    {
//        testCCoreApplicationOneIterationOneTopic( period[i], repetitions );
//    }

//    int numbOfTopics{ 100 };
//    testCCoreApplicationOneIterationManyTopics( numbOfTopics );
//}

//void SdkTestUnits::testCCoreApplication2()
//{
//    QVector<int> period{ 50, 100 };
//    int repetitions{ 500 };
//    for ( int i = 0; i < period.size(); i++ )
//    {
//        testCCoreApplicationOneIterationOneTopic( period[i], repetitions );
//    }
//}

QList <const QMetaObject *> SdkTestUnits::getTopicsListForTest()
{
    QList <const QMetaObject *> mqttTopicsListForTest;

    if ( m_addClassesFlag == false )
    {
        addClasses();
        m_addClassesFlag = true;
    }

    for ( auto & parent : mqttTopicsList )
    {
        //поиск топиков без потомков
        bool parentFlag{ false };
        for ( auto & child : mqttTopicsList )
        {
            if ( child != parent )
            {
                if ( child->superClass() == parent )
                {
                    parentFlag = true;
                    break;
                }
            }
        }

        if ( parentFlag == false )
        {
            //проверка, что присутствует метод 'getEtalonJson'
            bool isEtalonJsonMethodFlag{ false };
            for  ( int methodId = parent->methodOffset(); methodId < parent->methodCount(); ++methodId )
            {
                QMetaMethod metaMethod = parent->method( methodId );
                QString methodName = metaMethod.name();
                if ( methodName == "getEtalonJson" )
                {
                    isEtalonJsonMethodFlag = true;
                }
            }

            if ( isEtalonJsonMethodFlag == true )
            {
                mqttTopicsListForTest << parent;
            }
            else
            {
                qDebug() << "Class '" << parent->className()
                         << "' missing method 'getEtalonJson'!";
            }
        }
    }
    return mqttTopicsListForTest;
}

QList < CMqttTopic * > SdkTestUnits::getTopicsObjects()
{
    QList <const QMetaObject *> mqttTopicsListForTest;
    mqttTopicsListForTest = getTopicsListForTest();

    QList < CMqttTopic * > mqttTopicsObjects;

    QObject * obj{ nullptr };
    CMqttTopic * topic{ nullptr };

    for ( auto & topicForTest : mqttTopicsListForTest )
    {
        obj = nullptr;
        topic = nullptr;
        obj = topicForTest->newInstance();
        topic = static_cast< CMqttTopic * >( obj );
        if ( topic == nullptr )
        {
            qDebug() << "\nFailed to create topic object!";
        }
        else
        {
            mqttTopicsObjects.append( topic );
        }
    }
    return mqttTopicsObjects;
}

void SdkTestUnits::testTopicsEtalonJson()
{
    qDebug() << "\n\n\n" << Q_FUNC_INFO;
    QList < CMqttTopic * > mqttTopicsObjects;
    mqttTopicsObjects = getTopicsObjects();
    for ( auto & topic : mqttTopicsObjects )
    {
        qDebug() << "\n\nTOPIC  " << topic->topicName();
        /*! @brief Возможны три случая:
         * 1. Ошибка разбора - сырые данные
         * 2. Объект
         * 3. массив
         * */
        QByteArray etalonJson{}, receivedJson{};
        etalonJson = topic->getEtalonJson();
        QJsonParseError parsErr{};
        QJsonDocument etalonDoc = QJsonDocument::fromJson( etalonJson, & parsErr );
        topic->initFromData( etalonJson );
        receivedJson = topic->data();
        if ( parsErr.error == QJsonParseError::NoError )
        {
        /*! Проверка JSON */
            if ( etalonDoc.isEmpty() || etalonDoc.isNull() )
            {
                qDebug() << "\nJson object is empty or not initialized!";
            }
            else if ( etalonDoc.isObject() || etalonDoc.isArray() )
            {
                qDebug() << "\nEtalon json = " << etalonJson
                         << "\n\nReceived json = " <<  receivedJson;
                QCOMPARE( QJsonDocument::fromJson( etalonJson ),
                          QJsonDocument::fromJson( receivedJson ));
            }
            else
            {
                qDebug() << Q_FUNC_INFO << ": UNACCOUNTED SITUATION!";
            }
        }
        else
        {
        /*! Проверка RAW data */
            qDebug() << "\nTopic returned raw data"
                     << "\nEtalon data = " << etalonJson
                     << "\n\nReceived data = " <<  receivedJson;
            QCOMPARE( etalonJson, receivedJson );
        }
    }
}

//void SdkTestUnits::printDebugInfo( int period, int repetitions )
//{
//    qDebug() << "\nPeriod between publications, ms = " << period << "  Number of messages published = " << repetitions;
//}

//void SdkTestUnits::testCCoreApplicationOneIterationOneTopic( int period, int repetitions )
//{
//    //(1)
//    qDebug() << "\n\nTest for methods 'subscribeToMqttTopic'/'unsubscribeFromMqttTopic'"
//             << "\nMany subscriptions/unsubscribes and retain messages";
//    testCCoreApplicationManyRepetitions( period, repetitions );

//    //(2)
//    qDebug() << "\n\nTest for methods 'subscribeToMqttTopic'/'unsubscribeFromMqttTopic'"
//             << "\nOne subscription and many publications";
//    testCCoreApplicationManyPublications( period, repetitions );

//    //(4)
//    qDebug() << "\n\nTest for method 'publishToMqttTopic'"
//             << "\nOne subscription and many publications";
//    testCCoreApplicationManyPublications2( period, repetitions );
//}

//void SdkTestUnits::testCCoreApplicationOneIterationManyTopics( int numbOfTopics )
//{
//    //(3)
//    qDebug() << "\n\nTest for methods 'subscribeToMqttTopic'/'unsubscribeFromMqttTopic'"
//             << "\nSubscription to many topics"
//             << "\nNumber of topics = " << numbOfTopics;
//    testCCoreApplicationManyTopics( numbOfTopics );

//    //(5)
//    qDebug() << "\n\nTest for method 'publishToMqttTopic'"
//             << "\nSubscription to many topics"
//             << "\nNumber of topics = " << numbOfTopics;
//    testCCoreApplicationManyTopics2( numbOfTopics );
//}


///************************************   тесты ***************************************************/
//// (1)
//void SdkTestUnits::testCCoreApplicationManyRepetitions(int period, int repetitions)
//{
//    printDebugInfo( period, repetitions );
//    int argc = 2;
//    char *argv[] = {(char*)"./test_app", (char *)"--quiet"};
//    QCoreApplication app(argc, argv);
//    Q_UNUSED(app);

//     QString topicName{ "topic1" };

//    test_CCoreApplicationPublisher publisher( topicName, this );
//    test_CCoreApplication testedObj( topicName, this );
//    test_CCoreApplication testedObj2( "" );


//    connect( & testedObj, & test_CCoreApplication::startTestRetain,
//             this, [ & ]()
//    {
//        publisher.startTestRetain( period,  repetitions );
//    }, Qt::QueuedConnection );


//    connect( & publisher, & test_CCoreApplicationPublisher::messagePublished,
//             & testedObj, & test_CCoreApplication::messagePublishedHandler);

//    connect( & testedObj, & test_CCoreApplication::endIteration,
//             & publisher, & test_CCoreApplicationPublisher::endIteration );

//    connect( & publisher, & test_CCoreApplicationPublisher::stopTestRetain,
//             & testedObj, & test_CCoreApplication::stopTestRetain);

//    CMainHandler::runController( &testedObj );

//    testedObj.calcDelays();

//    qDebug() << "\nNumber of received messages = " << testedObj.getNumbOfMessages()
//             << "\nNumber of missed messages = " << repetitions - testedObj.getNumbOfMessages()
//             << "\nMaximum delay, ms (time from publication to subscription) = " << testedObj.getPubSubMaxDelay()
//             << "\nMessage number with maximum delay = " << testedObj.getNumbMsgPubSubDelay()
//             << "\nMaximum delay, ms (time from subscription to receiving) = " << testedObj.getSubReceiveMaxDelay()
//             << "\nMessage number with maximum delay = " << testedObj.getNumbMsgSubReceiveDelay();
//    QCOMPARE( testedObj.getNumbOfMessages(), repetitions );
//}

//// (2)
//void SdkTestUnits::testCCoreApplicationManyPublications( int period, int repetitions )
//{
//    printDebugInfo( period, repetitions );
//    int argc = 2;
//    char *argv[] = {(char*)"./test_app", (char *)"--quiet"};
//    QCoreApplication app(argc, argv);
//    Q_UNUSED(app);

//    const QString topicName{ "topic2" };

//    test_CCoreApplication2 testedObj( topicName, this );
//    test_CCoreApplicationPublisher2 publisher( topicName, this );

//    connect( & testedObj, & test_CCoreApplication2::startTest,
//             this, [ & ] ()
//    {
//        publisher.startTest( period, repetitions );
//    }, Qt::QueuedConnection );

//    connect( & publisher, & test_CCoreApplicationPublisher2::stopTest,
//             & testedObj, & test_CCoreApplication2::stopTest,
//             Qt::QueuedConnection );

//    CMainHandler::runController( &testedObj );

//    testedObj.calcDelays();

//    qDebug() << "\nNumber of received messages = " << testedObj.getNumbOfMessages()
//             << "\nNumber of missed messages = " << repetitions - testedObj.getNumbOfMessages()
//             << "\nMaximum delay, ms (time from publication to receiving) = " << testedObj.getMaxDelay()
//             << "\nMessage number with maximum delay = " << testedObj.getNumbMessageMaxDelay();

//    QCOMPARE( testedObj.getNumbOfMessages(), repetitions );
//}

//// (4)
//void SdkTestUnits::testCCoreApplicationManyPublications2( int period, int repetitions )
//{
//    printDebugInfo( period, repetitions );
//    int argc = 2;
//    char *argv[] = {(char*)"./test_app", (char *)"--quiet"};
//    QCoreApplication app(argc, argv);
//    Q_UNUSED(app);

//    QString topicName{ "topic4" };
//    CTestTopic topic( topicName );

//    test_CCoreApplicationSubscriber4 subscriber( topic, this );

//    subscriber.subscribeToTopic();
//    QCOMPARE( subscriber.test_subscribeToTopic(), true );

//    test_CCoreApplication4 testedObj( topic, period, repetitions, this );
//    CMainHandler::runController( & testedObj );

//    subscriber.unsubscribeFromTopic();
//    QCOMPARE( subscriber.test_unsubscribeFromTopic(), true );

//    subscriber.calcDelays();
//    qDebug() << "\nNumber of received messages = " << subscriber.getNumbOfReceivedMessages()
//             << "\nNumber of missed messages = " << repetitions - subscriber.getNumbOfReceivedMessages()
//             << "\nMaximum delay, ms (time from publication to receiving) = " << subscriber.getMaxDelay()
//             << "\nMessage number with maximum delay = " << subscriber.getNumbMessageMaxDelay();
//    QCOMPARE( subscriber.getNumbOfReceivedMessages(), repetitions );
//}

//// (3)
//void SdkTestUnits::testCCoreApplicationManyTopics( int numbOfTopics )
//{
//    int argc = 2;
//    char *argv[] = {(char*)"./test_app", (char *)"--quiet"};
//    QCoreApplication app(argc, argv);
//    Q_UNUSED(app);

//    QStringList topicsNames{};
//    QList < CTestTopic > topics{};

//    for ( int i = 0; i < numbOfTopics; i ++ )
//    {
//        QString topicName{};
//        topicName = "topic3_" + QString::number( i );

//        topicsNames.append( topicName );
//        topics.append( CTestTopic( topicName ) );
//    }
//    test_CCoreApplication3 testedObj( topics, this );
//    test_CCoreApplicationPublisher3 publisher( topicsNames, this );

//    connect( & testedObj, & test_CCoreApplication3::startTest,
//             & publisher, & test_CCoreApplicationPublisher3::startTest,
//             Qt::QueuedConnection);

//    connect( & publisher, & test_CCoreApplicationPublisher3::stopTest,
//             & testedObj, & test_CCoreApplication3::stopTest);

//    CMainHandler::runController( & testedObj );

//    testedObj.calcMaxDelay();

//    qDebug() << "\nNumber of missed messages (for all topics) = " << testedObj.getNumbOfMissedMessages()
//             << "\nMaximum delay, ms (time from publication to receiving) = " << testedObj.getMaxDelay()
//             << "\nTopic name with maximum delay = " << testedObj.getTopicNameMaxDelay();
//    QCOMPARE( testedObj.getNumbOfMissedMessages(), 0 );
//}

//// (5)
//void SdkTestUnits::testCCoreApplicationManyTopics2( int numbOfTopics )
//{
//    int argc = 2;
//    char *argv[] = {(char*)"./test_app", (char *)"--quiet"};
//    QCoreApplication app(argc, argv);
//    Q_UNUSED(app);

//    QList < CTestTopic > topics{};
//    const QString beginTopicName{ "topic5_" };

//    for ( int i = 0; i < numbOfTopics; i++ )
//    {
//        topics.append( CTestTopic( beginTopicName + QString::number( i )) );
//    }
//    test_CCoreApplicationSubscriber5 subscriber( topics, this );
//    subscriber.subscribeToTopics();
//    QCOMPARE( subscriber.test_subscribeToTopics(), true );

//    test_CCoreApplication5 testedObj( topics, this );
//    CMainHandler::runController( & testedObj );

//    subscriber.unsubscribeFromTopics();
//    QCOMPARE( subscriber.test_unsubscribeFromTopics(), true );

//    subscriber.calcDelays();
//    qDebug() << "\nNumber of missed messages (for all topics) = " << subscriber.getNumbOfMissedMessages()
//             << "\nMaximum delay, ms (time from publication to receiving) = " << subscriber.getMaxDelay()
//             << "\nTopic name with maximum delay = " << subscriber.getTopicNameMaxDelay();
//    QCOMPARE( subscriber.getNumbOfMissedMessages(), 0 );

//}
/************************************************************************************************/

/*
+CAbstractController.o
CAbstractSettingsProvider.o
CCameraState.o
CConsoleLogger.o
CDatabaseConnectionSettings.o
CDatabaseHandler.o
CDiagnosticEntryInfo.o
CFileUtils.o
CFormalizedMessage.o
CGuiMessage.o
cinformatormisc.o
CInformatorStationInfo.o
CInformatorSubrouteInfo.o
CKeepAliveHandler.o
CKeepAliveTopicMessage.o
CLoggerHandler.o
CLoggerMessage.o
+CMainHandler.o
CMqttClientHandler.o
CMqttConnectionSettings.o
CMqttMessage.o
CMqttTopicConnection.o
CMqttTopicsHandler.o
CMultyChannelAudioPlayer.o
CRecorderUploadStatus.o
CSensorState.o
CSettingsDatabaseProvider.o
CSettingsEntry.o
CSettingsHandler.o
CSettings.o
CSettingsVariantExtender.o
CStringUtils.o
CTermoState.o
CTopicAudioTrackToPlay.o
CTopicAudioTrackToPlayStatus.o
CTopicAudioVolume.o
CTopicConnectionState.o
CTopicConnectionVoiceDialCmd.o
CTopicConnectionVoiceDialState.o
CTopicDiagnosticInfo.o
CTopicInformatorBusesList.o
CTopicInformatorChangeModeCmd.o
CTopicInformatorPlayDoorsCmd.o
CTopicInformatorPlayStationCmd.o
CTopicInformatorRouteState.o
CTopicInformatorSelectedMode.o
CTopicInformatorSelectedRouteCmd.o
CTopicInformatorSelectedSubrouteCmd.o
CTopicIntegrationValidators.o
CTopicLiazErrors.o
CTopicLiazKneeling.o
CTopicLoggerGuiMessage.o
CTopicNavigationValueGnssState.o
CTopicOnvifFoundCameras.o
CTopicRecorderCameras.o
CTopicRecorderRecords.o
CTopicRecorderUploadCmd.o
CTopicRecorderUploadStatus.o
CTopicSettingsSettingChangeCmd.o
CTopicStmDiscretValue.o
CTopicStmDoubleValue.o
CTopicStmRoFwWersion.o
CTopicStmRoMfi.o
CTopicStmRoOneWire.o
CTopicSystemHealthHddPrepare.o
CTopicSystemHealthHddStatus.o
CTopicSystemHealthHddStatusVolume.o
CTopicSystemHealthHddStatusVolumePart.o
CTopicSystemRunasroot.o
CTopicTrackerServerIps.o
CTopicValidatorsPassangerCounter.o
CTopicValidatorsStatus.o
CTopicValueEntry.o
CUtils.o
*/

//    publisher.startTestRetain( period, 10 );
//    QEventLoop loop{};
//    QObject::connect( & publisher, & test_CCoreApplicationPublisher::testRetainFinished,
//                      & loop, &QEventLoop::quit );
//    loop.exec( QEventLoop::EventLoopExec );


/*
    inline bool publishToMqttTopic(const QString & topicName,
                            const QJsonDocument &data)
    inline bool publishToMqttTopic(const QString & topicName,
                            const QByteArray &data)
    inline bool publishToMqttTopic(const QString & topicName,
                            const QJsonDocument &data,
                            bool retain )
    inline bool publishToMqttTopic(const QString & topicName,
                            const QByteArray &data,
                            bool retain )
    bool publishToMqttTopic(const QString & topicName,
                            const QJsonDocument &data,
                            bool retain,
                            quint8 qos);
    bool publishToMqttTopic(const QString & topicName,
                            const QByteArray &data,
                            bool retain,
                            quint8 qos);
    bool subscribeToMqttTopic( const QString & topicName );
    bool unsubscribeFromMqttTopic( const QString & topicName );
    virtual void OnMqttMessageReceived( const QString & topic,
                              const QJsonObject & json );

    inline void setSettingsOwner(const char *owner)
    inline QString getSettingsOwner(void) const
    inline void setModuleName(const char *moduleName)
    virtual bool getDefaultSettings(QList<CSettingsEntry> &list);
    virtual void loadSettings(QList < CSettingsEntry > &list);
    inline QList < CSettingsEntry > getSettingsList() const
    inline void setSettingsEntry(CSettingsEntry &entry) const
    static QByteArray makeSimpleJson( const QVariant & value );
    void attach();
    void detach();
    virtual void mqttConnected();
    virtual void mqttDisconnected();
    virtual void mqttMessageReceived( const CMqttMessage & message );
*/

//    QVERIFY(t.run() == true);
    //    CAbstractController    c;
