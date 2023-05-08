#include "routesNetworkParser.h"

#include <QJsonObject>
#include <QJsonDocument>

RoutesNetworkParser::RoutesNetworkParser(const QString &topicName)
    : m_topicName(topicName)
{

}

void RoutesNetworkParser::parseData(const QByteArray &payload)
{
    m_url = "";
    m_sha256 = "";
    m_file_date = QDateTime();

    QJsonDocument json_doc = QJsonDocument::fromJson(QString(payload).toUtf8());

    if (!json_doc.isNull())
    {
        QJsonObject json_obj = json_doc.object();

        if (!json_obj.isEmpty())
        {
            if (json_obj.contains("date") && json_obj.contains("url") && json_obj.contains("crc"))
            {
                m_file_date = QDateTime::fromString(json_obj["date"].toString(), Qt::ISODate);
                m_url = json_obj["url"].toString();
                m_sha256 = json_obj["crc"].toString();
            }
        }
    }
}
