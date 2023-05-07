#ifndef CGUICVSSTATETOPIC_H
#define CGUICVSSTATETOPIC_H

#include <Mqtt/Topics/CGrGuiTopic.h>

/** ***********************************************************************
 * @class CGuiCvsStateTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CGuiCvsStateTopic : public CGrGuiTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CGuiCvsStateTopic() : CGrIncarnetInspectorTopic( "STATE" ) {}
#endif

public:
    explicit CGuiCvsStateTopic();
    explicit CGuiCvsStateTopic( const int inWork );
    explicit CGuiCvsStateTopic( const QByteArray &data );

    inline int inWork() const
    {
        return m_inWork;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setInWork( const int value );

protected:
    virtual const QByteArray prepareData() const override;
    virtual bool parseData( const QByteArray & data ) override;

private:
    int m_inWork{ 0 };
};

#endif // CGUICVSSTATETOPIC_H
