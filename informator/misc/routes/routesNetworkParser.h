#pragma once

#include <QString>
#include <QDateTime>

class RoutesNetworkParser
{
public:
    RoutesNetworkParser(const QString& topicName);
    void parseData(const QByteArray& payload);
    inline QDateTime fileDate() const {return m_file_date;}
    inline QString url() const {return m_url;}
    inline QString crc() const {return m_sha256;}
    inline QString topicName() const {return m_topicName;}
private:
    QString     m_topicName;
    QDateTime   m_file_date;
    QString     m_url;
    QString     m_sha256;
};

