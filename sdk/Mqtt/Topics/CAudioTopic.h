#ifndef __AUDIO_TOPIC_H__
#define __AUDIO_TOPIC_H__

#include <QHash>
#include <QVariant>

#include <Mqtt/CMqttTopic.h>

class CAudioTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CAudioTopic() : CMqttTopic() {}
#endif

    public:
    
        static const char *FIELD_SENDER; // = "Sender";
        static const char *FIELD_VALUE; // = "Value";
        static const char *FIELD_NEED_TO_SAVE; // = "NeedToSave";
        static const char *FIELD_ACTION; // = "Action";
        static const char *FIELD_TRACK; // = "Track";
        static const char *FIELD_UUID; // = "Uuid";
        static const char *FIELD_TARGET;
        static const char *FIELD_STATUS;
        static const char *FIELD_PRIORITY; // = "Priority"
        static const char *FIELD_DELAY; // = "Delay"

        CAudioTopic(const QString &subTopicName);
        virtual ~CAudioTopic();

        void setField(const QString &name, const QVariant &value);
        void setField(const QString &name, const QString &value);
        void setField(const char *name, const char *value);
        void setField(const char *name, const QString &value);
        void setField(const QString &name, int value);
        void setField(const QString &name, double value);
        const QVariant getField(const QString &name) const;

    protected:
        virtual const QByteArray prepareData() const;
        virtual bool parseData( const QByteArray & data );

    protected:
        QHash<QString, QVariant>    m_fields;
};

#endif
