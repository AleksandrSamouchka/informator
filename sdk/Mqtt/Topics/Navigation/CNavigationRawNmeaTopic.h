#ifndef CNAVIGATIONRAWNMEATOPIC_H
#define CNAVIGATIONRAWNMEATOPIC_H

#include <Mqtt/Topics/CGrNavigationTopic.h>

/** ***********************************************************************
 * @class CNavigationRawNmeaTopic
 * @brief класс, умеющий работать с настройкой в топике
 ** ***********************************************************************/
class CNavigationRawNmeaTopic : public CGrNavigationTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
#endif

public:
#ifdef MNT_TOPICS_TEST
    Q_INVOKABLE
#endif
    explicit CNavigationRawNmeaTopic();
    explicit CNavigationRawNmeaTopic( const QString & rawData );
    explicit CNavigationRawNmeaTopic( const QByteArray & data );

    inline const QString & rawData() const
    {
        return m_rawData;
    }
#ifdef MNT_TOPICS_TEST
public:
    Q_INVOKABLE QByteArray getEtalonJson() override;
#endif

public:
    void setRawDataString( const QString & rawData );

protected:
    const QByteArray prepareData() const override;
    bool parseData( const QByteArray & data ) override;

private:
    QString m_rawData{};
};


#endif // CNAVIGATIONRAWNMEATOPIC_H
