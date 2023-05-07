#ifndef CLEDTESTTOPIC_H
#define CLEDTESTTOPIC_H

#include <Mqtt/Topics/CGrLedTopic.h>

/** ***********************************************************************
 * @class CLedTestTopic
 ** ***********************************************************************/
class CLedTestTopic : public CGrLedTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    explicit CLedTestTopic();
    explicit CLedTestTopic( const CGrLedTopic::ETargets target );
    explicit CLedTestTopic( const QByteArray &data );

/*    inline CGrLedTopic::ETargets getTarget() const
    {
        return m_target;
    }
*/
    inline const QString & getNumberText() const
    {
        return m_numberText;
    }
    inline const QString & getStringText() const
    {
        return m_stringText;
    }

#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
//    void setTarget( const CGrLedTopic::ETargets value );
    void setNumberText( const QString & value );
    void setStringText( const QString & value );

protected:
    virtual const QByteArray prepareData() const override;
    virtual bool parseData( const QByteArray & data ) override;

private:
//    CGrLedTopic::ETargets m_target{};
    QString m_numberText{};
    QString m_stringText{};
};

#endif // CLEDTESTTOPIC_H
