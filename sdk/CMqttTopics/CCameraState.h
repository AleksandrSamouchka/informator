#ifndef CCAMERASTATE_H
#define CCAMERASTATE_H

#include <QtCore/QObject>

/** ***********************************************************************
 * @class CCameraState
 * @brief структура, описывающая состояние камеры
 ** ***********************************************************************/
class CCameraState
{
public:
    enum class EStreamForPlaying
    {
        primary = 0,
        secondary,
    };

    enum class EActiveState
    {
        inactive = 0,
        unknown,
        active,
    };

public:
    EActiveState m_activeState{ EActiveState::inactive };
    int m_cameraNumber{ 0 };
    QString m_cameraName{};
    QString m_primaryStream{};
    QString m_secondaryStream{};
    EStreamForPlaying m_streamForPlaying{ EStreamForPlaying::secondary };

    CCameraState() = default;
    CCameraState( const CCameraState & other ) = default;
    CCameraState( const CCameraState::EActiveState activeState,
                  const int cameraNumber,
                  const QString cameraName,
                  const QString primaryStream = QString{},
                  const QString secondaryStream = QString{},
                  const EStreamForPlaying streamForPlaying = EStreamForPlaying::secondary );

    static CCameraState::EActiveState getActiveState( const QString & str );
};

#endif // CCAMERASTATE_H
