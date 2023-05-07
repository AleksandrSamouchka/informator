#ifndef CTOPICLIAZERRORS_H
#define CTOPICLIAZERRORS_H

#include <CMqttTopics/CTopicValueEntry.h>

/** ***********************************************************************
 * @class CTopicLiazErrors
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CTopicLiazErrors : public CTopicValueEntry
{
public:
    int m_dtcoDisconnect{ 0 };
    int m_dtcoYellow{ 0 };
    int m_dtcoRed{ 0 };
    int m_lowCoolant{ 0 };
    int m_ecas{ 0 };
    int m_transmission{ 0 };
    int m_cpc{ 0 };
    int m_scr{ 0 };
    int m_ebss{ 0 };
    int m_ec{ 0 };
    int m_vcu{ 0 };
    int m_cbcu{ 0 };
    int m_absYellow{ 0 };
    int m_checkYellow{ 0 };
    int m_checkRed{ 0 };

public:
    CTopicLiazErrors( const QByteArray &json );
    CTopicLiazErrors();
    CTopicLiazErrors( const CTopicLiazErrors & other ) = default;

    bool initFromData( const CTopicValueEntry &value ) override;
    virtual QByteArray prepareData() const override;
    virtual ~CTopicLiazErrors() override = default;

    bool operator == ( const CTopicLiazErrors & right ) const;
    bool operator != ( const CTopicLiazErrors & right ) const;
};

#endif // CTOPICLIAZERRORS_H
