#include "CSettingsEntry.h"
#include <CLoggerMessage/CLoggerMessage.h>
#include <QtCore/QString>
#include <QtCore/QStringList>

const QStringList applicationSettingsTypes = QStringList
{
    "text",
    "number"
};


/** ***********************************************************************
 * @fn CSettingsEntry  [конструктор]
 ** ***********************************************************************/
CSettingsEntry::CSettingsEntry( const QString &name,
                                const QString &humanName,
                                const QString &value,
                                const QString &type,
                                const QString &description,
                                const QString &module,
                                const bool isPrivate,
                                const QString &subgroupName,
                                const QString &subgroupHumanName ) :
    m_name( name ),
    m_humanName( humanName ),
    m_value( value ),
    m_type( type ),
    m_module( module ),
    m_description( description ),
    m_private( isPrivate ),
    m_subgroupName( subgroupName ),
    m_subgroupHumanName( subgroupHumanName )
{ }

CSettingsEntry::CSettingsEntry( const QString &name,
                                const QString &value,
                                const bool isPrivate,
                                const QString &subgroupName,
                                const QString &subgroupHumanName ) :
    m_name( name ),
    m_value( value ),
    m_private( isPrivate ),
    m_subgroupName( subgroupName ),
    m_subgroupHumanName( subgroupHumanName )
{ }

/** ***********************************************************************
 * @fn setters
 * @brief слоты для задания свойств класса
 ** ***********************************************************************/
void CSettingsEntry::setName(const QString &name)
{
    m_name = name;
}

void CSettingsEntry::setHumanName(const QString &humanName)
{
    m_humanName = humanName;
}

void CSettingsEntry::setValue(const QString &value)
{
    m_value = value;
}

void CSettingsEntry::setType(const QString &type)
{
    m_type = type;
}

void CSettingsEntry::setModule(const QString &module)
{
    m_module = module;
}

void CSettingsEntry::setDescription(const QString &description)
{
    m_description = description;
}

void CSettingsEntry::setPrivate(const bool isPrivate)
{
    m_private = isPrivate;
}

void CSettingsEntry::setSubgroupName(const QString &name)
{
    m_subgroupName = name;
}

void CSettingsEntry::setSubgroupHumanName(const QString &humanName)
{
    m_subgroupHumanName = humanName;
}

/** ***********************************************************************
 * @fn getters
 * @brief методы для чтения свойств класса
 ** ***********************************************************************/
QString CSettingsEntry::getName() const
{
    return m_name;
}

QString CSettingsEntry::getHumanName() const
{
    return m_humanName;
}

QString CSettingsEntry::getValue() const
{
    return m_value;
}

QString CSettingsEntry::getType() const
{
    return m_type;
}

QString CSettingsEntry::getModule() const
{
    return m_module;
}

QString CSettingsEntry::getDescription() const
{
    return m_description;
}

bool CSettingsEntry::isPrivate() const
{
    return m_private;
}

QString CSettingsEntry::getSubgroupName() const
{
    return m_subgroupName;
}

QString CSettingsEntry::getSubgroupHumanName() const
{
    return m_subgroupHumanName;
}

QString CSettingsEntry::getTypeString(const EApplicationSettingsTypes type)
{
    return applicationSettingsTypes.value( type, "" );
}

/** ***********************************************************************
 * @fn isValid      [const]
 * @brief метод для проверки корректного заполнения полей описания настройки.
 *  Проверяется name, humanName и type - чтобы были не пустые
 * @param QString                   - параметр (наименование настройки) поддерживается только латиница!
 * @return CSettingsEntry
 ** ***********************************************************************/
bool CSettingsEntry::isValid() const
{
    bool res{ false };
    if ( ! m_name.isEmpty() )
    {
        res = true;
    }
    return res;
}

