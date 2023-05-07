#ifndef __APPCONTROLLER_HPP__
#define __APPCONTROLLER_HPP__

#include <CAbstractController.h>
#include <CSettingsEntry.h>
#include <CSettingsHandler.h>
#include <CSettingsVariantExtender.h>


class AppController : public CAbstractController
{
    Q_OBJECT

public:
    explicit AppController( QObject * parent = nullptr );
    virtual ~AppController();

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

};

#endif