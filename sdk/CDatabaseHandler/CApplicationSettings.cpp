#include "CApplicationSettings.h"
#include <CLoggerMessage.h>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

namespace DB_SETT
{
    const QStringList applicationSettingsTypes{
        QStringLiteral( "text" ),
        QStringLiteral( "number" )
    };
}

/** ***********************************************************************
 * @fn CApplicationSettings  [конструктор]
 ** ***********************************************************************/
CApplicationSettings::CApplicationSettings( const QString &parameter,
                                            const QString &humanName,
                                            const QVariant &value,
                                            const QString &type ) :
    m_parameter( parameter ),
    m_humanName( humanName ),
    m_value( value ),
    m_type( type )
{ }

/** ***********************************************************************
 * @fn setters
 * @brief слоты для задания свойств класса
 ** ***********************************************************************/
void CApplicationSettings::setParameter(const QString &parameter)
{
    m_parameter = parameter;
}

void CApplicationSettings::setHumanName(const QString &humanName)
{
    m_humanName = humanName;
}

void CApplicationSettings::setValue(const QVariant &value)
{
    m_value = value;
}

void CApplicationSettings::setType(const QString &type)
{
    m_type = type;
}

/** ***********************************************************************
 * @fn getters
 * @brief методы для чтения свойств класса
 ** ***********************************************************************/
QString CApplicationSettings::getParameter() const
{
    return m_parameter;
}

QString CApplicationSettings::getHumanName() const
{
    return m_humanName;
}

QVariant CApplicationSettings::getValue() const
{
    return m_value;
}

QString CApplicationSettings::getType() const
{
    return m_type;
}

/** ***********************************************************************
 * @fn isValid      [const]
 * @brief метод для проверки корректного заполнения полей описания настройки.
 *  Проверяется parameter, humanName и type - чтобы были не пустые
 * @param QString                   - параметр (наименование настройки) поддерживается только латиница!
 * @return CApplicationSettings
 ** ***********************************************************************/
bool CApplicationSettings::isValid() const
{
    bool res{ false };
    if ( ! m_parameter.isEmpty() && ! m_humanName.isEmpty() && ! m_type.isEmpty() )
    {
        res = true;
    }
    return res;
}

/** ***********************************************************************
 * @fn isParameter      [const]
 * @brief метод для сравнения наименования настройки (параметра)
 * @param QString                   - параметр (наименование настройки) поддерживается только латиница!
 * @return bool
 ** ***********************************************************************/
bool CApplicationSettings::isParameter(const QString &parameter) const
{
    bool res{ false };
    res = m_parameter == parameter;
    return res;
}

QString DB_SETT::getTypeString(const EApplicationSettingsTypes type)
{
    return applicationSettingsTypes.value( type, "" );
}
