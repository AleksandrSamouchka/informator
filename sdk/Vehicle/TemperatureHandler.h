#pragma once

#include <QObject>
#include <QPointer>

class TemperatureHandler : public QObject
{
    Q_OBJECT
    enum class ETempSource : int
    {
        can = 0,
        oneWire
    };

public:
    static TemperatureHandler& getInstance()
    {
        static TemperatureHandler instance;
        return instance;
    }
    static void init(QObject* app);
    inline int getInsideTemp() const {return m_tempInside;}
    inline int getOutsideTemp() const {return m_tempOutside;}
signals:
    void temperatureChanged();
protected slots:
    void onSettingsChange();
    void onRoOneWireMsg();
    void onCanSalonTempTopic();
    void onCanAmbientTempTopic();
protected:
    explicit TemperatureHandler(QObject *parent = nullptr);
    ~TemperatureHandler() override {}
private:
    TemperatureHandler (const TemperatureHandler& other) = delete;
    TemperatureHandler& operator = (const TemperatureHandler& other) = delete;
    TemperatureHandler (const TemperatureHandler&& other) = delete;
    TemperatureHandler& operator = (const TemperatureHandler&& other) = delete;
    void subscribeToTopics();
    /*!
     * \brief readTempSensorsSettings - read settings from database. Get temp source
     * CAN or one wire
     */
    void readTempSensorsSettings();
private:
    int         m_tempInside{0};
    int         m_tempOutside{0};
    QStringList m_termInsideNames;
    QString     m_termOutsideName;
    ETempSource m_tempSourceInside{ETempSource::oneWire};
    ETempSource m_tempSourceOutside{ETempSource::oneWire};
    QObject*    m_app;
};





