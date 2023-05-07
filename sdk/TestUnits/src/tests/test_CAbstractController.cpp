#include <QtCore/QCoreApplication>

#include "test_CAbstractController.hpp"

test_CAbstractController::test_CAbstractController( QObject *parent ):
    CAbstractController(parent),
    m_exitCode(0),
    m_timerId(-1),
    m_defaultSettingsFlag(false),
    m_loadSettingsFlag(false),
    m_onAttachFlag(false),
    m_mqttConnectedFlag(false)

{}

test_CAbstractController::~test_CAbstractController()
{
}

void test_CAbstractController::mqttConnected()
{
    m_mqttConnectedFlag = true;
}

void test_CAbstractController::OnMqttMessageReceived( const QString & topic,
                      const QJsonObject & json )
{
    Q_UNUSED(topic);
    Q_UNUSED(json);
}

bool test_CAbstractController::getDefaultSettings(QList<CSettingsEntry> &list)
{
    const auto typeString{ CSettingsEntry::getTypeString( CSettingsEntry::SettingsType_text ) };
    const auto moduleName{ QCoreApplication::applicationName() };

    CSettingsEntry  tmpEntry;

    tmpEntry.setType( typeString );
    tmpEntry.setModule( moduleName );

    tmpEntry.setSubgroupName("test_subgroup_name");
    tmpEntry.setSubgroupHumanName("test subgroup title");

    tmpEntry.setName( "test_param" );
    tmpEntry.setHumanName( "Test param title" );
    tmpEntry.setDescription( "test param description");
    tmpEntry.setValue( CSettingsVariantExtender{ 284 }.getJson() );
    list.append(tmpEntry);

    m_defaultSettingsFlag = true;

    return false; // do not override
}

void test_CAbstractController::loadSettings(QList < CSettingsEntry > &list)
{
    Q_UNUSED(list);
    m_loadSettingsFlag = true;
}

void test_CAbstractController::onAttach()
{
    m_onAttachFlag = true;
    m_timerId = startTimer(1000);
}

void test_CAbstractController::onDetach()
{
    m_onDetachFlag = true;
}

/*********   *****************/

void test_CAbstractController::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    QCoreApplication::exit(m_exitCode);
}
