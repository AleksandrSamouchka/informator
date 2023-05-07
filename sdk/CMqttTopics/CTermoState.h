#ifndef CTERMOSTATE_H
#define CTERMOSTATE_H

#include <QtCore/QObject>

/** ***********************************************************************
 * @class CTermoState
 * @brief класс, содержащий информацию об остановочном пункте
 ** ***********************************************************************/
class CTermoState
{

public:
    QString m_name{};
    QString m_id{};
    double m_value{ 0.0 };

public:
    CTermoState(){}
    explicit CTermoState( const QString &name,
                          const QString &id,
                          double value );
    virtual ~CTermoState() = default;
};


#endif // CTERMOSTATE_H
