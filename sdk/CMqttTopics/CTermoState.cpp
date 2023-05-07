#include "CTermoState.h"

CTermoState::CTermoState( const QString &name,
                          const QString &id,
                          double value ) :
    m_name( name ),
    m_id( id ),
    m_value{ value }
{ }
