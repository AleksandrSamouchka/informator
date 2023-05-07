#include "COnvifCameraParams.h"

#include <QtCore/QStringList>

static const QStringList STATUS_STRINGS
{
    "НЕИЗВЕСТНО",
    "НЕИЗВЕСТНОЕ УСТРОЙСТВО",
    "ОШИБКИ",
    "ОШИБКА АВТОРИЗАЦИИ",
    "ОТКЛЮЧЕНО",
    "В РАБОТЕ",
};

QString COnvifCameraParams::statusString(const COnvifCameraParams::EStatus status)
{
    return STATUS_STRINGS.value( static_cast< int >( status ), "НЕИЗВЕСТНО" );
}
