#ifndef CLEDLOADFONTSTATUSTOPIC_H
#define CLEDLOADFONTSTATUSTOPIC_H

#include <Mqtt/Topics/CGrLedTopic.h>

/** ***********************************************************************
 * @class CLedLoadFontStatusTopic
 ** ***********************************************************************/
class CLedLoadFontStatusTopic : public CGrLedTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
    enum EStatus
    {
        unknown = 0,
        inProgress,
        finished,
        failed,

        statusesCount,
    };

public:
    explicit CLedLoadFontStatusTopic();
    explicit CLedLoadFontStatusTopic( const QByteArray &data );

/*    inline CGrLedTopic::ETargets getTarget() const
    {
        return m_target;
    }
*/
    inline EStatus getStatus() const
    {
        return m_status;
    }
    inline int getTotalPieceCounter() const
    {
        return m_totalPieceCount;
    }
    inline int getCurrentPieceNumber() const
    {
        return m_currentPieceNumber;
    }

#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
//    void setTarget(const ETargets value );

    void setStatus( const EStatus value );
    void setTotalPieceCounter( const int value );
    void setCurrentPieceNumber( const int value );

protected:
    virtual const QByteArray prepareData() const override;
    virtual bool parseData( const QByteArray & data ) override;

private:
//    CGrLedTopic::ETargets m_target{};
    EStatus m_status{ EStatus::finished };
    int m_totalPieceCount{ 0 };
    int m_currentPieceNumber{ 0 };
};

#endif // CLEDLOADFONTSTATUSTOPIC_H
