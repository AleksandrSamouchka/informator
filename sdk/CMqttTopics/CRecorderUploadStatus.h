#ifndef CRECORDERUPLOADSTATUS_H
#define CRECORDERUPLOADSTATUS_H

#include <QtCore/QList>

/** ***********************************************************************
 * @class CRecorderUploadStatus
 * @brief класс, содержащий информацию о статусе выгрузки видео роликов
 ** ***********************************************************************/
class CRecorderUploadStatus
{
public:
    enum class EUploadStatus
    {
        ok = 0,
        fail,
    };

public:
    QString m_id{};
    int m_progress{ 0 };
    EUploadStatus m_status{ EUploadStatus::fail };
    QString m_errString{};

public:
    CRecorderUploadStatus() = default;
    explicit CRecorderUploadStatus( const QString &id,
                                    int progress,
                                    const QString &statusStr,
                                    const QString &errString = QString{} );
    virtual ~CRecorderUploadStatus() = default;
};

#endif // CRECORDERUPLOADSTATUS_H
