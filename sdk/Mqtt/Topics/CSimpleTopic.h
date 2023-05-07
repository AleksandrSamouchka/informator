#ifndef SIMPLE_TOPIC_H
#define SIMPLE_TOPIC_H

#include <QDebug>

#include <Mqtt/CMqttTopic.h>

static const char *fieldName = "Value";

template <class T>
class CSimpleTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
public:
    CSimpleTopic() : CMqttTopic() {}
#endif
    public:
        CSimpleTopic(const QString &topicName, const char* a_fieldName = fieldName) : CMqttTopic(topicName),
            m_fieldName(a_fieldName)
        {
            setValue(0);
        }

        CSimpleTopic(const QString &topicName, const T &value, const char* a_fieldName = fieldName) : CMqttTopic(topicName),
            m_fieldName(a_fieldName)
        {
            setValue(value);
        }
        virtual ~CSimpleTopic() {}

        virtual void setValue(const T &value)
        {
            m_value = value;
        }

        T getValue() const
        {
            return m_value;
        }

    protected:
        virtual bool parseData( const QByteArray & data ) override
        {
            bool result = false;
            QJsonParseError jsonErr{};
            QJsonObject jsonObj( QJsonDocument::fromJson( data, & jsonErr ).object() );

            if ( jsonErr.error != QJsonParseError::NoError )
            {
                printJsonParseErrorMessage( data, jsonErr );
            }
            else
            {
                if ( ! jsonObj.contains( m_fieldName ) )
                {
                    printUnknownFieldsListMessage( jsonObj.keys() );
                }
                else
                {
//qDebug() << jsonObj.value( fieldName ).toDouble();
                    double value = jsonObj.value( m_fieldName ).toDouble();
//qDebug() << value;
//qDebug() << QVariant(value).value<T>();
                    setValue(QVariant(value).value<T>());
                    result = true;
                }
            }

            setValidState( result );

            return result;
        }

        virtual const QByteArray prepareData() const
        {
//qDebug() << "m_value = " << m_value;
            QJsonObject jsonObj{ { m_fieldName, m_value } };
//qDebug() << "json = " << QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact);
            return QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact) ;
        }

    private:
/*
        typedef enum
        {
            none_type = 0,
            int_type,
            str_type
        } ArgType;
*/
        T m_value;
        const char* m_fieldName;
};


#endif

