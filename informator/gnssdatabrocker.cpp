#include "gnssdatabrocker.h"

GnssDataBrocker::GnssDataBrocker(int intervalMs, QObject *parent):
        QObject(parent),
        m_intervalMs(intervalMs)
{}

// ОСТАНОВКИ Шаг 2 - Данная функция вызвана InformatorHandler::onGnssStateTopic()
void GnssDataBrocker::gnssDataReceived(const GnssState_t& gnssState)
{
    killTimer(timerIdent);
    timerIdent = startTimer(m_intervalMs);
    emit gnssData(gnssState, false);
}

void GnssDataBrocker::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    emit gnssData(CInformatorMisc::gnssIncorrectState(), true);
}
