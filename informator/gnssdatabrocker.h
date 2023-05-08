#pragma once

// Qt
#include <QObject>
#include "misc/informator/cinformatormisc.h"

/** ***********************************************************************
 * @class GnssDataBrocker
 * @brief Класс принимает топик gnss и вырабатывает соответствующий ему
 * сигнал. Если топик не приходит более чем через указанную в конструкторе
 * длительность, то вырабатывается тот же сигнал с неверными данными gnss.
 * Нужен этот класс для того, чтобы гид информатора не реже указанного в
 * конструкторе интервал получал сигнал о приходе данных gnss и корректно
 * обновлял у себя время (брал или gnss или системное), и уже по нему
 * проставлял время прибытия на остановку.
 * @todo
 *
 ** ***********************************************************************/
class GnssDataBrocker : public QObject
{
    Q_OBJECT
    signals:
        void gnssData(GnssState_t gnssState, bool noData);

    public:
        GnssDataBrocker(int intervalMs, QObject *parent = nullptr);
        void start()
        {
            timerIdent = startTimer(m_intervalMs);
        }

    protected:
        void timerEvent(QTimerEvent *event) override;

    public slots:
        void gnssDataReceived(const GnssState_t &gnssState);

    private:
        int m_intervalMs;
        int timerIdent;
};
