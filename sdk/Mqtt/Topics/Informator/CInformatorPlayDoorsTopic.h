#ifndef CINFORMATORPLAYDOORSTOPIC_H
#define CINFORMATORPLAYDOORSTOPIC_H

#include <Mqtt/Topics/CGrInformatorTopic.h>

/** ***********************************************************************
 * @class CInformatorPlayDoorsTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CInformatorPlayDoorsTopic : public CGrInformatorTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CInformatorPlayDoorsTopic( );
    explicit CInformatorPlayDoorsTopic( const int stationNum );
    explicit CInformatorPlayDoorsTopic( const QByteArray &data );

    inline int stationNum() const
    {
        return m_stationNum;
    }

#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setStationNum( const int value );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    int m_stationNum{};
};

#endif // CINFORMATORPLAYDOORSTOPIC_H
