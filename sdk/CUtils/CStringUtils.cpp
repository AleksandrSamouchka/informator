#include "CStringUtils.h"

#include <QString>
/*

CStringUtils::StringStream CStringUtils::ss( const char * s )
{
    StringStream result;
    result.m_string = QString::fromUtf8( s );
    return result;
}

CStringUtils::StringStream::operator QString() const
{
    return m_string;
}

void CStringUtils::StringStream::advance()
{
    const auto i{ m_string.indexOf( QLatin1Char{ '%' } ) };
    auto j{ i };

    if ( i >= 0 ) {
        while ( ++j < m_string.length() && m_string[ j ].isDigit() ) {}
        m_string.replace( i + 1, j - i - 1,
                          QString::number( m_string.mid( i + 1, j - i - 1 ).toInt() - 1 ) );
    }
}
*/

QString CStringUtils::choppedString( const char *s, size_t length )
{
    QString result(s);
    result.chop( static_cast< int >( length ) );
    return result;
}
