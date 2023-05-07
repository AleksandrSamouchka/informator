#ifndef CTOPICTESTRESULT_H
#define CTOPICTESTRESULT_H

#include <QString>
#include <QObject>

/** **********************************************************
 * @class CReceivedMessage
 * @brief Для сбора информации о принятом сообщении
 * @author Пищулина И
 * **********************************************************/
class CReceivedMessage
{
public:
    CReceivedMessage( int numbMessage, qint64 publishTs,
                      qint64 receiveTs, int receivingDelay )
        : m_numbMessage{ numbMessage },
          m_publishTs{ publishTs },
          m_receiveTs{ receiveTs },
          m_receivingDelay{ receivingDelay }
    {
    }

    int m_numbMessage{};
    qint64 m_publishTs{}, m_receiveTs{};
    int m_receivingDelay{};
};


/** **********************************************************
 * @class CTopicTestResult
 * @brief Для сбора информации о топике
 * @author Пищулина И
 * **********************************************************/
class CTopicTestResult
{
public:
    CTopicTestResult( QString topicName );

    void addMessage( int numbMessage, qint64 publishTs, qint64 receiveTs, int receivingDelay);

    inline int getMaxReceivingDelay() const
    {
        return m_maxDelay;
    }

    inline int getNumbMessageMaxDelay() const
    {
        return m_numbMessageMaxDelay;
    }

    inline int getNumbOfMessages() const
    {
        return m_receivedMessages.size();
    }

    inline QString getTopicName() const
    {
        return m_topicName;
    }

    void calcMaxDelay();

private:
    QString m_topicName{};
    QList < CReceivedMessage > m_receivedMessages{};

    int m_numbMessageMaxDelay{};
    int m_maxDelay{};
};

#endif // CTOPICTESTRESULT_H
