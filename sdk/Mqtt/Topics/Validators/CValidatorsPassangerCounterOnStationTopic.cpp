#include "CValidatorsPassangerCounterOnStationTopic.h"

namespace
{
    const char* TOPIC = "PASSANGER_COUNTER_ON_STATION_MGT";
    const char* FIELD_DOOR_INFO = "doorInfo";
    const char* DOOR = "door";
    const char* FIELD_PASSANGER_ENTERED = "passangerEntered";
    const char* FIELD_PASSANGER_LEFT = "passangerLeft";
}

CValidatorsPassangerCounterOnStationTopic::CValidatorsPassangerCounterOnStationTopic() :
    CGrValidatorsTopic(TOPIC)
{}

CValidatorsPassangerCounterOnStationTopic::CValidatorsPassangerCounterOnStationTopic(const QByteArray &data) :
    CGrValidatorsTopic(TOPIC, data)
{}

void CValidatorsPassangerCounterOnStationTopic::setIrmaPresentDoor(uint8_t value)
{
    m_irma_present_door = value;
}

void CValidatorsPassangerCounterOnStationTopic::setInPassangerCounters(const QList<uint8_t> values)
{
    m_inPassangerCounters = values;
}

void CValidatorsPassangerCounterOnStationTopic::setOutPassangerCounters(const QList<uint8_t> values)
{
    m_outPassangerCounters = values;
}

const QByteArray CValidatorsPassangerCounterOnStationTopic::prepareData() const
{
    QJsonArray  jsonArrInCounter{},
                jsonArrOutCounter{};

    for (int i = 0; i < m_inPassangerCounters.size(); ++i)
    {
        QJsonObject jsonObj({ {DOOR + QString::number(i + 1), m_inPassangerCounters.at(i)} });
        jsonArrInCounter.append(jsonObj);
    }

    for (int i = 0; i < m_outPassangerCounters.size(); ++i)
    {
        QJsonObject jsonObj({ {DOOR + QString::number(i + 1), m_outPassangerCounters.at(i)} });
        jsonArrOutCounter.append(jsonObj);
    }

    QJsonObject jsonMainObj({ {FIELD_DOOR_INFO, m_irma_present_door},
                              {FIELD_PASSANGER_ENTERED, jsonArrInCounter},
                              {FIELD_PASSANGER_LEFT, jsonArrOutCounter}});

    return QJsonDocument(jsonMainObj).toJson(QJsonDocument::Compact);
}

bool CValidatorsPassangerCounterOnStationTopic::parseData(const QByteArray &data)
{
    m_inPassangerCounters.clear();
    m_outPassangerCounters.clear();
    bool res{false};

    const QJsonObject jsonMainObj(QJsonDocument::fromJson(data).object());
    if (!jsonMainObj.isEmpty())
    {
        if (jsonMainObj.contains(FIELD_DOOR_INFO) &&
            jsonMainObj.contains(FIELD_PASSANGER_ENTERED) &&
            jsonMainObj.contains(FIELD_PASSANGER_LEFT))
        {
            m_irma_present_door = jsonMainObj.value(FIELD_DOOR_INFO).toInt();

            int doorIdx = 1;

            for (const auto i : jsonMainObj.value(FIELD_PASSANGER_ENTERED).toArray())
            {
                QJsonObject jsonObj = i.toObject();

                m_inPassangerCounters.append(jsonObj.value(DOOR + QString::number(doorIdx)).toInt());

                ++doorIdx;
            }

            doorIdx = 1;

            for (const auto i : jsonMainObj.value(FIELD_PASSANGER_LEFT).toArray())
            {
                QJsonObject jsonObj = i.toObject();

                m_outPassangerCounters.append(jsonObj.value(DOOR + QString::number(doorIdx)).toInt());

                ++doorIdx;
            }

            res = true;
        }
        else
        {
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else
    {
        printEmptyJsonMessage();
    }

    return setValidState(res);
}
