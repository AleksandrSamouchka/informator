#include "CRecorderUploadStatus.h"

namespace
{
    const QStringList STATUSES{ "OK", "FAIL" };
}

CRecorderUploadStatus::CRecorderUploadStatus( const QString &id,
                                              int progress,
                                              const QString &statusStr,
                                              const QString &errString ) :
    m_id{ id },
    m_progress{ progress },
    m_status{ static_cast< EUploadStatus >( STATUSES.indexOf( statusStr ) < 0 ? 1 : STATUSES.indexOf( statusStr ) ) },
    m_errString{ errString }
{ }
