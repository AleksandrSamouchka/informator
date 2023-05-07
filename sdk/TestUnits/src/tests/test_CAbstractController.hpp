#ifndef __TEST_C_ABSTRACTCONTROLLER_HPP__
#define __TEST_C_ABSTRACTCONTROLLER_HPP__

#include <QObject>
#include <QTest>


#include <CMainHandler/CAbstractController.h>
#include <CSettingsHandler/CSettingsEntry/CSettingsEntry.h>
#include <CSettingsHandler/CSettingsHandler.h>
#include <CSettingsHandler/CSettingsEntry/CSettingsVariantExtender.h>


class test_CAbstractController : public CAbstractController
{
    Q_OBJECT
/**   Overrided methods                                    **/
    public:
        explicit test_CAbstractController( QObject * parent = nullptr );
        virtual ~test_CAbstractController();

    protected:
        virtual void OnMqttMessageReceived( const QString & topic,
                      const QJsonObject & json ) override;
        virtual bool getDefaultSettings(QList<CSettingsEntry> &list) override;
        virtual void loadSettings(QList < CSettingsEntry > &list) override;

    private:
        virtual void onAttach() override;
        virtual void onDetach() override;

    private slots:
        virtual void mqttConnected() override;

/** ------------------------------------------------------- **/



/**  Test methods                                           **/
    protected:
        void timerEvent(QTimerEvent *event) override;

    public:  
        void setExitCode(int code)
        {
            m_exitCode = code;
        }

        inline bool test_getDefaultSettings() const 
        {
            return m_defaultSettingsFlag == true;
        }

        inline bool test_loadSettings() const 
        {
            return m_loadSettingsFlag == true;
        }

        inline bool test_onAttach() const 
        {
            return m_onAttachFlag == true;
        }

        inline bool test_onDetach() const 
        {
            return m_onAttachFlag == true;
        }

        inline bool test_mqttConnected() const 
        {
            return m_mqttConnectedFlag == true;
        }

  
    private:
        int  m_exitCode;
        int  m_timerId;
        bool m_defaultSettingsFlag;
        bool m_loadSettingsFlag;
        bool m_onAttachFlag;
        bool m_onDetachFlag;
        bool m_mqttConnectedFlag;
        
/** ------------------------------------------------------- **/
};

#endif
