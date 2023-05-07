#include "CSettingsVariantExtender.h"
#include <CConsoleLogger/CConsoleLogger.h>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QStringList>

#include <QtCore/QSize>
#include <QtCore/QUrl>

/*#ifdef  QT_GUI_LIB
#include <QtGui/QColor>
#include <QtGui/QFont>
#endif
*/

namespace
{
    const char *SETT_MAIN_KEY  = "value";

    const char *SETT_KEY_COLOR = "color";
    const char *SETT_KEY_DAY = "day";
    const char *SETT_KEY_MONTH = "month";
    const char *SETT_KEY_YEAR = "year";
    const char *SETT_KEY_HOUR = "hour";
    const char *SETT_KEY_MINUTE = "minute";
    const char *SETT_KEY_SECOND = "second";
    const char *SETT_KEY_FAMILY = "family";
    const char *SETT_KEY_ITALIC = "italic";
    const char *SETT_KEY_SIZE = "size";
    const char *SETT_KEY_WEIGHT = "weight";
    const char *SETT_KEY_HEIGHT = "height";
    const char *SETT_KEY_WIDTH = "width";
    const char *SETT_KEY_URL = "url";
    const char *SETT_KEY_CAMERAURL = "cameraUrl";

    const QStringList SETT_COLOR_KEYS
    {
        SETT_KEY_COLOR
    };
    const QStringList SETT_DATE_KEYS
    {
        SETT_KEY_DAY,
        SETT_KEY_MONTH,
        SETT_KEY_YEAR
    };
    const QStringList SETT_TIME_KEYS
    {
        SETT_KEY_HOUR,
        SETT_KEY_MINUTE,
        SETT_KEY_SECOND
    };
    const QStringList SETT_DATETIME_KEYS
    {
        SETT_KEY_DAY,
        SETT_KEY_HOUR,
        SETT_KEY_MINUTE,
        SETT_KEY_MONTH,
        SETT_KEY_SECOND,
        SETT_KEY_YEAR
    };
    const QStringList SETT_FONT_KEYS
    {
        SETT_KEY_FAMILY,
        SETT_KEY_ITALIC,
        SETT_KEY_SIZE,         /*!< размер в пикселях! */
        SETT_KEY_WEIGHT
    };
    const QStringList SETT_SIZE_KEYS
    {
        SETT_KEY_HEIGHT,
        SETT_KEY_WIDTH
    };
    const QStringList SETT_URL_KEYS
    {
        SETT_KEY_URL
    };
    const QStringList SETT_CAMERAURL_KEYS
    {
        SETT_KEY_CAMERAURL
    };

}

/** ***********************************************************************
 * @brief Конструктор - инициализация структурой RAW настройки
 ** ***********************************************************************/
CSettingsVariantExtender::CSettingsVariantExtender( const CSettingsEntry &setting ) :
    m_value( setting.getValue().toUtf8() )
{ }

CSettingsVariantExtender::CSettingsVariantExtender(const QByteArray &json) :
    m_value( json )
{ }

CSettingsVariantExtender::CSettingsVariantExtender( bool value )
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( value ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QString &value, const ESupportTypes type)
{
    QJsonObject jsonMainObj{};
    if ( type == ESupportTypes::Type_color )
    {
        QJsonObject jsonObj{QPair<QString, QJsonValue>{ SETT_KEY_COLOR, value }};
        jsonMainObj.insert( SETT_MAIN_KEY, QJsonValue(jsonObj) );
    }
    else
    {
        jsonMainObj.insert( SETT_MAIN_KEY, QJsonValue(value) );
    }
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

//CSettingsVariantExtender::CSettingsVariantExtender( const QString value )
//{
//    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( value ) } };
//    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
//}

CSettingsVariantExtender::CSettingsVariantExtender( const char *value )
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( value ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender( int value )
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( value ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender( double value )
{
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( value ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QVariantList &value)
{
    const QJsonArray jsonArr( QJsonArray::fromVariantList( value ) );
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonArr ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QVariantMap &value)
{
    const QJsonObject jsonObj( QJsonObject::fromVariantMap( value ) );
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonObj ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QDate &value)
{
    const QJsonObject jsonObj{ {
            { SETT_KEY_YEAR, QJsonValue( value.year() ) },
            { SETT_KEY_MONTH, QJsonValue( value.month() ) },
            { SETT_KEY_DAY, QJsonValue( value.day() ) }
        } };
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonObj ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QTime &value)
{
    const QJsonObject jsonObj{ {
            { SETT_KEY_HOUR, QJsonValue( value.hour() ) },
            { SETT_KEY_MINUTE, QJsonValue( value.minute() ) },
            { SETT_KEY_SECOND, QJsonValue( value.second() ) }
        } };
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonObj ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QDateTime &value)
{
    const QJsonObject jsonObj{ {
            { SETT_KEY_YEAR, QJsonValue( value.date().year() ) },
            { SETT_KEY_MONTH, QJsonValue( value.date().month() ) },
            { SETT_KEY_DAY, QJsonValue( value.date().day() ) },
            { SETT_KEY_HOUR, QJsonValue( value.time().hour() ) },
            { SETT_KEY_MINUTE, QJsonValue( value.time().minute() ) },
            { SETT_KEY_SECOND, QJsonValue( value.time().second() ) }
        } };
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonObj ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QSize &value)
{
    const QJsonObject jsonObj{ {
            { SETT_KEY_HEIGHT, QJsonValue( value.height() ) },
            { SETT_KEY_WIDTH, QJsonValue( value.width() ) }
        } };
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonObj ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QUrl &value, const ESupportTypes type )
{
    QJsonObject jsonObj{};
    if ( type == ESupportTypes::Type_url )
    {
        jsonObj.insert( SETT_KEY_URL, value.toString() );
    }
    else if ( type == ESupportTypes::Type_cameraUrl )
    {
        jsonObj.insert( SETT_KEY_CAMERAURL, value.toString() );
    }
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonObj ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

/*#ifdef  QT_GUI_LIB
CSettingsVariantExtender::CSettingsVariantExtender(const QColor value)
{
    const QJsonObject jsonObj{ QPair<QString, QJsonValue>{ SETT_KEY_COLOR, QJsonValue( value.name() ) } };
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonObj ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}

CSettingsVariantExtender::CSettingsVariantExtender(const QFont value)
{
    const QJsonObject jsonObj{ {
            { SETT_KEY_FAMILY, QJsonValue( value.family() ) },
            { SETT_KEY_ITALIC, QJsonValue( value.italic() ) },
            { SETT_KEY_SIZE, QJsonValue( value.pixelSize() ) },
            { SETT_KEY_WEIGHT, QJsonValue( value.weight() ) }
        } };
    const QJsonObject jsonMainObj{ QPair<QString, QJsonValue>{ SETT_MAIN_KEY, QJsonValue( jsonObj ) } };
    m_value = QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact );
}
#endif
*/
/** ***********************************************************************
 * @fn getValueType      [const]
 * @brief метод для получения типа данных настройки. Если тип данных не известен -
 *  возвращается QVariant::Invalid
 * @param none
 * @return ESupportTypes
 ** ***********************************************************************/
CSettingsVariantExtender::ESupportTypes CSettingsVariantExtender::getValueType() const
{
    ESupportTypes valueType{ ESupportTypes::Type_invalid };
    if ( ! m_value.isEmpty() )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonValue jsonMainVal( jsonMainObj.value( SETT_MAIN_KEY ) );
            const QJsonValue::Type mainValType{ jsonMainVal.type() };
            if ( mainValType == QJsonValue::Null || mainValType == QJsonValue::Undefined )
            {
                PRINT_CONSOLE_MESSAGE_WARN( "Неизвестный тип данных" );
            }
            else if ( mainValType == QJsonValue::Bool )
            {
                valueType = ESupportTypes::Type_bool;
            }
            else if ( mainValType == QJsonValue::Double )
            {
                valueType = ESupportTypes::Type_double;
            }
            else if ( mainValType == QJsonValue::String )
            {
                valueType = ESupportTypes::Type_string;
            }
            else if ( mainValType == QJsonValue::Array )
            {
                valueType = ESupportTypes::Type_list;
            }
            else if ( mainValType == QJsonValue::Object )
            {
                valueType = ESupportTypes::Type_map;
                const QJsonObject jsonObj( jsonMainVal.toObject() );
                QStringList jsonObjKeys( jsonObj.keys() );
                jsonObjKeys.sort( Qt::CaseInsensitive );
                if ( jsonObjKeys == SETT_COLOR_KEYS )
                {
                    valueType = ESupportTypes::Type_color;
                }
                else if ( jsonObjKeys == SETT_DATE_KEYS )
                {
                    valueType = ESupportTypes::Type_date;
                }
                else if ( jsonObjKeys == SETT_TIME_KEYS )
                {
                    valueType = ESupportTypes::Type_time;
                }
                else if ( jsonObjKeys == SETT_DATETIME_KEYS )
                {
                    valueType = ESupportTypes::Type_dateTime;
                }
                else if ( jsonObjKeys == SETT_FONT_KEYS )
                {
                    valueType = ESupportTypes::Type_font;
                }
                else if ( jsonObjKeys == SETT_SIZE_KEYS )
                {
                    valueType = ESupportTypes::Type_size;
                }
                else if ( jsonObjKeys == SETT_URL_KEYS )
                {
                    valueType = ESupportTypes::Type_url;
                }
                else if ( jsonObjKeys == SETT_CAMERAURL_KEYS )
                {
                    valueType = ESupportTypes::Type_cameraUrl;
                }
            }
        }
    }
    return valueType;
}

/** ***********************************************************************
 * @fn getJsonValue      [const]
 * @brief метод для получения variant значения в виде json пакета
 * @param none
 * @return QByteArray
 ** ***********************************************************************/
QString CSettingsVariantExtender::getJson() const
{
    return QString::fromUtf8( m_value );
}

/** ***********************************************************************
 * @fn isValid      [const]
 * @brief метод для проверки значение не пустое, готов для разбора данных
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CSettingsVariantExtender::isValid() const
{
    return ! m_value.isEmpty();
}

/** ***********************************************************************
 * @brief Методы, выполняют попытку распарсить данные и возвратить величину
 * указанного типа.
 * Перед использованием того или иного метода следует вначале выполнить проверку
 * типа значения на соответствие одному из известных типов
 ** ***********************************************************************/
bool CSettingsVariantExtender::toBool( bool * ok ) const
{
    bool res{ false };
    bool value{ false };
    if ( getValueType() == ESupportTypes::Type_bool )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonValue jsonVal( jsonMainObj.value( SETT_MAIN_KEY ) );
            value = jsonVal.toBool();
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QString CSettingsVariantExtender::toString( bool * ok ) const
{
    bool res{ false };
    QString value{};
    if ( getValueType() == ESupportTypes::Type_string )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonValue jsonVal( jsonMainObj.value( SETT_MAIN_KEY ) );
            value = jsonVal.toString();
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

int CSettingsVariantExtender::toInt( bool * ok ) const
{
    bool res{ false };
    int value{ 0 };
    CSettingsVariantExtender::ESupportTypes type = getValueType();
    if ( type == ESupportTypes::Type_int ||
        type == ESupportTypes::Type_double )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonValue jsonVal( jsonMainObj.value( SETT_MAIN_KEY ) );
            value = jsonVal.toInt();
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}


double CSettingsVariantExtender::toDouble( bool * ok ) const
{
    bool res{ false };
    double value{ 0 };
    if ( getValueType() == ESupportTypes::Type_double )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonValue jsonVal( jsonMainObj.value( SETT_MAIN_KEY ) );
            value = jsonVal.toDouble();
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QVariantList CSettingsVariantExtender::toList( bool * ok ) const
{
    bool res{ false };
    QVariantList value{};
    if ( getValueType() == ESupportTypes::Type_list )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonValue jsonVal( jsonMainObj.value( SETT_MAIN_KEY ) );
            value = jsonVal.toArray().toVariantList();
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QVariantMap CSettingsVariantExtender::toMap( bool * ok ) const
{
    bool res{ false };
    QVariantMap value{};
    if ( getValueType() == ESupportTypes::Type_map )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonValue jsonVal( jsonMainObj.value( SETT_MAIN_KEY ) );
            value = jsonVal.toObject().toVariantMap();
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QDate CSettingsVariantExtender::toDate( bool * ok  ) const
{
    bool res{ false };
    QDate value{};
    if ( getValueType() == ESupportTypes::Type_date )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value.setDate( jsonObj.value( SETT_KEY_YEAR ).toInt(),
                           jsonObj.value( SETT_KEY_MONTH ).toInt(),
                           jsonObj.value( SETT_KEY_DAY ).toInt() );
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QTime CSettingsVariantExtender::toTime( bool * ok ) const
{
    bool res{ false };
    QTime value{};
    if ( getValueType() == ESupportTypes::Type_time )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value = QTime( jsonObj.value( SETT_KEY_HOUR ).toInt(),
                           jsonObj.value( SETT_KEY_MINUTE ).toInt(),
                           jsonObj.value( SETT_KEY_SECOND ).toInt() );
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QDateTime CSettingsVariantExtender::toDateTime( bool * ok ) const
{
    bool res{ false };
    QDateTime value{};
    if ( getValueType() == ESupportTypes::Type_dateTime )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value.setDate( QDate ( jsonObj.value( SETT_KEY_YEAR ).toInt(),
                                   jsonObj.value( SETT_KEY_MONTH ).toInt(),
                                   jsonObj.value( SETT_KEY_DAY ).toInt() ) );
            value.setTime( QTime( jsonObj.value( SETT_KEY_HOUR ).toInt(),
                                  jsonObj.value( SETT_KEY_MINUTE ).toInt(),
                                  jsonObj.value( SETT_KEY_SECOND ).toInt() ) );
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QSize CSettingsVariantExtender::toSize( bool * ok ) const
{
    bool res{ false };
    QSize value{};
    if ( getValueType() == ESupportTypes::Type_size )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value = QSize( jsonObj.value( SETT_KEY_WIDTH ).toInt(),
                           jsonObj.value( SETT_KEY_HEIGHT ).toInt() );
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QUrl CSettingsVariantExtender::toUrl( bool * ok ) const
{
    bool res{ false };
    QUrl value{};
    if ( getValueType() == ESupportTypes::Type_url )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value.setUrl( jsonObj.value( SETT_KEY_URL ).toString() );
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QUrl CSettingsVariantExtender::toCameraUrl( bool * ok ) const
{
    bool res{ false };
    QUrl value{};
    if ( getValueType() == ESupportTypes::Type_cameraUrl )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value.setUrl( jsonObj.value( SETT_KEY_CAMERAURL ).toString() );
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QString CSettingsVariantExtender::toColor( bool * ok ) const
{
    bool res{ false };
    QString value{};
    if ( getValueType() == ESupportTypes::Type_color )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value = jsonObj.value( SETT_KEY_COLOR ).toString();
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

/*#ifdef  QT_GUI_LIB
QColor CSettingsVariantExtender::toColor( bool * ok ) const
{
    bool res{ false };
    QColor value{};
    if ( getValueType() == ESupportTypes::Type_color )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value.setNamedColor( jsonObj.value( SETT_KEY_COLOR ).toString() );
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}

QFont CSettingsVariantExtender::toFont( bool * ok ) const
{
    bool res{ false };
    QFont value{};
    if ( getValueType() == ESupportTypes::Type_font )
    {
        const QJsonObject jsonMainObj( QJsonDocument::fromJson( m_value ).object() );
        if ( ! jsonMainObj.isEmpty() && jsonMainObj.contains( SETT_MAIN_KEY ) )
        {
            const QJsonObject jsonObj( jsonMainObj.value( SETT_MAIN_KEY ).toObject() );
            value = QFont( jsonObj.value( SETT_KEY_FAMILY ).toString(),
                           jsonObj.value( SETT_KEY_SIZE ).toInt(),
                           jsonObj.value( SETT_KEY_WEIGHT ).toInt(),
                           jsonObj.value( SETT_KEY_ITALIC ).toBool() );
            value.setPixelSize( jsonObj.value( SETT_KEY_SIZE ).toInt() );
            res = true;
        }
    }
    if ( ok )
    {
        *ok = res;
    }
    return value;
}
#endif
*/
