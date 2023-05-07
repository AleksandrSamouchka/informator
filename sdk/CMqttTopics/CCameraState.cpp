#include "CCameraState.h"

namespace
{
    const QStringList ACTIVE_STATE_STR{ "INACTIVE", "UNKNOWN", "ACTIVE" };
}



/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CCameraState::CCameraState(const EActiveState activeState,
                            const int cameraNumber,
                            const QString cameraName,
                            const QString primaryStream,
                            const QString secondaryStream,
                            const EStreamForPlaying streamForPlaying ) :
    m_activeState{ activeState },
    m_cameraNumber{ cameraNumber },
    m_cameraName{ cameraName },
    m_primaryStream{ primaryStream },
    m_secondaryStream{ secondaryStream },
    m_streamForPlaying{ streamForPlaying }
{ }

CCameraState::EActiveState CCameraState::getActiveState(const QString & str)
{
    int index{ ACTIVE_STATE_STR.indexOf( str ) };
    if ( index < 0 )
    {
        return CCameraState::EActiveState::inactive;
    }
    else
    {
        return static_cast< CCameraState::EActiveState >( index );
    }
}
