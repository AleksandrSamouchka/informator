#ifndef CSETTINGSVARIANTEXTENDER_H
#define CSETTINGSVARIANTEXTENDER_H

#include <CSettingsHandler/CSettingsEntry/CSettingsEntry.h>
#include <QtCore/QVariant>

/** ***********************************************************************
 * @class CSettingsVariantExtender
 * @brief класс по превращению "сырой" RAW настройки в понятный тип
 ** ***********************************************************************/
class CSettingsVariantExtender
{
    QByteArray m_value = QByteArray{};

public:
/*! Список поддерживаемых типов данных - для справки */
    enum class ESupportTypes
    {
    /* основные типы */
        Type_bool = 0,
        Type_string,
        Type_int,
        Type_double,
    /* контейнеры */
        Type_list,
        Type_map,          /*!< Это довольно общий тип. Данный тип указывается если не удалось
                                                         * сопоставить список ключевых слов ни с одним из известных типов -
                                                         *      - в таком случае парсинг данных лежит на стороне пользователя
                                                         * На деле же должны быть использованы производные типы данных
                                                         */
    /* производные типы - от Type_Map */
        Type_color,
        Type_date,
        Type_time,
        Type_dateTime,
        Type_font,
        Type_size,
        Type_url,
        Type_cameraUrl,

    /* ошибка */
        Type_invalid,
    };

public:
    CSettingsVariantExtender() = default;
    CSettingsVariantExtender( const CSettingsVariantExtender & other ) = default;
    ~CSettingsVariantExtender() = default;
//    CSettingsVariantExtender & operator = ( const CSettingsVariantExtender & other )

    explicit CSettingsVariantExtender( const char * value);
    explicit CSettingsVariantExtender( const CSettingsEntry &setting );
    explicit CSettingsVariantExtender( const QByteArray &json );
    explicit CSettingsVariantExtender( bool value );
    explicit CSettingsVariantExtender( const QString &value, const ESupportTypes type = ESupportTypes::Type_string );
    explicit CSettingsVariantExtender( int value );
    explicit CSettingsVariantExtender( double value );
    explicit CSettingsVariantExtender( const QVariantList &value );
    explicit CSettingsVariantExtender( const QVariantMap &value );
    explicit CSettingsVariantExtender( const QDate &value );
    explicit CSettingsVariantExtender( const QTime &value );
    explicit CSettingsVariantExtender( const QDateTime &value );
    explicit CSettingsVariantExtender( const QSize &value );
    explicit CSettingsVariantExtender( const QUrl &value, const ESupportTypes type = ESupportTypes::Type_url );
/*#ifdef  QT_GUI_LIB
    explicit CSettingsVariantExtender( const QColor value );
    explicit CSettingsVariantExtender( const QFont value );
#endif
*/

    ESupportTypes getValueType() const;
    QString getJson() const;
    bool isValid() const;

/*! Методы получения значения */
    bool toBool( bool * ok = nullptr ) const;
    QString toString( bool * ok = nullptr ) const;
    int toInt( bool * ok = nullptr ) const;
    double toDouble( bool * ok = nullptr ) const;

    QVariantList toList( bool * ok = nullptr ) const;
    QVariantMap toMap( bool * ok = nullptr ) const;

    QDate toDate( bool * ok = nullptr ) const;
    QTime toTime( bool * ok = nullptr ) const;
    QDateTime toDateTime( bool * ok = nullptr ) const;
    QSize toSize( bool * ok = nullptr ) const;
    QUrl toUrl( bool * ok = nullptr ) const;
    QUrl toCameraUrl( bool * ok = nullptr ) const;
    QString toColor( bool * ok = nullptr ) const;

/*#ifdef  QT_GUI_LIB
    QColor toColor( bool * ok = nullptr ) const;
    QFont toFont( bool * ok = nullptr ) const;
#endif
*/
};

#endif // CSETTINGSVARIANTEXTENDER_H
