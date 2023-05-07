#include "CAbstractSettingsProvider.h"
#include <CSettingsHandler/CSettingsEntry/CSettingsEntry.h>

CAbstractSettingsProvider::CAbstractSettingsProvider(QObject *parent) :
    QObject(parent)
{
    static auto id1 = qRegisterMetaType<CSettingsEntry>("CSettingsEntry");
    static auto id2 = qRegisterMetaType<QList < CSettingsEntry >>("QList < CSettingsEntry >");
    Q_UNUSED( id1 )
    Q_UNUSED( id2 )
}
