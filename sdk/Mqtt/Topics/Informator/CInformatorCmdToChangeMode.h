#ifndef CINFORMATORCMDTOCHANGEMODE_H
#define CINFORMATORCMDTOCHANGEMODE_H

#include <Mqtt/Topics/CGrInformatorTopic.h>

/** ***********************************************************************
 * @class CInformatorCmdToChangeMode
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CInformatorCmdToChangeMode : public CGrInformatorTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    enum class EModes
    {
        Mode_auto = 0,
        Mode_manual,
        Mode_unknown
    };

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CInformatorCmdToChangeMode( );
    explicit CInformatorCmdToChangeMode( EModes mode );
    explicit CInformatorCmdToChangeMode( const QByteArray &data );
    inline EModes mode() const
    {
        return m_mode;
    }

#ifdef MNT_TOPICS_TEST
    public:
        Q_INVOKABLE QByteArray getEtalonJson() override;
#endif
public:
    void setMode( EModes mode );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    EModes m_mode{ EModes::Mode_auto };
};

#endif // CINFORMATORCMDTOCHANGEMODE_H
