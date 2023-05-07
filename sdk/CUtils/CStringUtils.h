#ifndef __CSTRING_UTILS_H__
#define __CSTRING_UTILS_H__

#include <QString>


class CStringUtils
{
    public:
        /*class StringStream
          {
              friend class CStringUtils;
          public:
              template<typename T>
              StringStream & operator <<( const T arg )
              {
                  m_string = m_string.arg( arg );
                  advance();
                  return * this;
              }
              operator QString() const;
          private:
              void advance();
          private:
              QString m_string;
          };

    static StringStream ss( const char * s );
    */
    static QString choppedString( const char *s,
                                      size_t length );
};

#endif
