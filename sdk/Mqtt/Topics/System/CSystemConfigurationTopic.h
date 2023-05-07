#ifndef SYSTEM_CONFIGURATION_TOPIC_H
#define SYSTEM_CONFIGURATION_TOPIC_H

#include <QHash>

#include <Mqtt/Topics/CSystemTopic.h>

class CSystemConfigurationTopic : public CSystemTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CSystemConfigurationTopic(); /*: CSystemTopic( "CONFIGURATION/" ) {}*/
#endif
    public:
        CSystemConfigurationTopic( const QString &subtopicName);
        CSystemConfigurationTopic( const QString &subtopicName, const QByteArray &data );

        template <class T>
        void setField(const QString &name, const T &value)
        {
            m_fields[name] = QVariant(value);
        }


        virtual ~CSystemConfigurationTopic();

    protected:
        virtual const QByteArray prepareData() const;

    private:
        QHash<QString, QVariant>    m_fields;
};

#endif
