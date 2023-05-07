#ifndef CSENSORSTATE_H
#define CSENSORSTATE_H

#include <QtCore/QObject>

/** ***********************************************************************
 * @class CSensorState
 * @brief класс, содержащий информацию об остановочном пункте
 ** ***********************************************************************/
class CSensorState
{
public:
    enum class ETypes
    {
        Type_analog = 0,
        Type_digital,
        Type_unknown,
        Type_tamper,
    };

public:
    ETypes m_type = ETypes::Type_unknown;
    double m_value{ 0.0 };

public:
    CSensorState(){}
    explicit CSensorState( const ETypes type,
                           const double value );
    virtual ~CSensorState() = default;
};

#endif // CSENSORSTATE_H
