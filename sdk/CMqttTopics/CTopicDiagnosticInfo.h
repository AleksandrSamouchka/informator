#ifndef CTOPICDIAGNOSTICINFO_H
#define CTOPICDIAGNOSTICINFO_H

/** ***********************************************************************
 * @class CTopicDiagnosticInfo
 * @brief Содержимое топика с информацией о состоянии модуля
 * (диагностика и готовность к работе)
 *
 * 1. Для работы в составе микросервиса:
 *      - наследуемся от данного класса
 *      -
 *
 * @author Мальнев А
 * @date 24-01-2019
 ** ***********************************************************************/

#include <CMqttTopics/CTopicValueEntry.h>
#include <CMqttTopics/modules/CDiagnosticEntryInfo.h>

/** ***********************************************************************
 * @class CTopicDiagnosticInfo
 ** ***********************************************************************/
class CTopicDiagnosticInfo
{
public:
    CTopicDiagnosticInfo() = default;
    CTopicDiagnosticInfo( const CTopicDiagnosticInfo & other ) = default;
/** ***********************************************************************
 * @fn CTopicDiagnosticInfo
 * @brief конструктор
 * @param QStringList   - список наименований модулей. Список создается сразу
 ** ***********************************************************************/
    CTopicDiagnosticInfo( const QStringList &subsystemNames, const QStringList &humanReadableNames );

    QByteArray prepareData();
    bool initFromData( const CTopicValueEntry &value );
public:
/** ***********************************************************************
 * @fn changeState, changeStatus
 * @brief методы изменения статуса и состояния
 * @param int       - номер подсистемы (см входной список подсистем в конструкторе)
 * @param CDiagnosticEntryInfo::EState или CDiagnosticEntryInfo::EStatus
 * @return bool     - флаг изменения (необходимости отправки данных)
 ** ***********************************************************************/
    bool changeState(const QString &subsystem, const CDiagnosticEntryInfo::EState state );
    bool changeStatus( const QString &subsystem, const CDiagnosticEntryInfo::EStatus status );

/** ***********************************************************************
 * @fn isChanges
 * @brief методы проверки на необходимость отправки данных
 * @param none
 * @return bool     - флаг изменения (необходимости отправки данных)
 ** ***********************************************************************/
    bool isChanges() const;

private:
    QMap < QString, CDiagnosticEntryInfo > mSubsystems{};

    bool mIsChanged{ true };
};

#endif // CTOPICDIAGNOSTICINFO_H

