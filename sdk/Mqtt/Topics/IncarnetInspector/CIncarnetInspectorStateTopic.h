#ifndef CINCARNETINSPECTORSTATETOPIC_H
#define CINCARNETINSPECTORSTATETOPIC_H

#include <Mqtt/Topics/CGrIncarnetInspectorTopic.h>

/** ***********************************************************************
 * @class CIncarnetInspectorStateTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CIncarnetInspectorStateTopic : public CGrIncarnetInspectorTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    Q_INVOKABLE CIncarnetInspectorStateTopic() : CGrIncarnetInspectorTopic( "STATE" ) {}
#endif

public:
    explicit CIncarnetInspectorStateTopic( const int enabled,
                                           const int inWork );
    explicit CIncarnetInspectorStateTopic();

    inline int enabled() const
    {
        return m_enabled;
    }
    inline int inWork() const
    {
        return m_inWork;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setEnabled( const int value );
    void setInWork( const int value );

protected:
    virtual const QByteArray prepareData() const override;
    virtual bool parseData( const QByteArray & data ) override;

private:
    int m_enabled{ 0 };
    int m_inWork{ 0 };
};

#endif // CINCARNETINSPECTORSTATETOPIC_H
