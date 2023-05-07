#ifndef CKEEPALIVETOPICMESSAGE_H
#define CKEEPALIVETOPICMESSAGE_H

#include <QtCore/QObject>

class CKeepAliveTopicMessage
{
public:
    CKeepAliveTopicMessage( const QString &payload );
    QByteArray prepareData() const;

private:
    QString mVersion;
    QString mPayload;
};

#endif // CKEEPALIVETOPICMESSAGE_H
