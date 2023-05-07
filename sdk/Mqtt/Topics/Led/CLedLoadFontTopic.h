#ifndef CLEDLOADFONTTOPIC_H
#define CLEDLOADFONTTOPIC_H

#include <Mqtt/Topics/CGrLedTopic.h>

/** ***********************************************************************
 * @class CLedLoadFontTopic
 ** ***********************************************************************/
class CLedLoadFontTopic : public CGrLedTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    explicit CLedLoadFontTopic();
    explicit CLedLoadFontTopic( CGrLedTopic::ETargets target,
                                const QString & fileName );
    explicit CLedLoadFontTopic( const QByteArray &data );

/*    inline CGrLedTopic::ETargets getTarget() const
    {
        return m_target;
    }
*/
    inline const QString & getFileName() const
    {
        return m_fileName;
    }

#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
//    void setTarget( ETargets value );
    void setFileName( const QString & value );

protected:
    virtual const QByteArray prepareData() const override;
    virtual bool parseData( const QByteArray & data ) override;

private:
//    CGrLedTopic::ETargets m_target{};
    QString m_fileName{};
};

#endif // CLEDLOADFONTTOPIC_H
