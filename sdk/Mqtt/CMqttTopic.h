#ifndef CMQTTTOPIC_H
#define CMQTTTOPIC_H

/** ***********************************************************************
 * @name CMqttTopic
 * @brief Базовый класс для MQTT топика
 * @author Искренков А
 * @date 27-08-2019
 ** ***********************************************************************/
#include "CMqttHandler/CMqttMessage.h"

#include <QtCore/QObject>
#include <QtCore/QString>

/** ***********************************************************************
 * @class CMqttTopic
 ** ***********************************************************************/
class CMqttTopic : public QObject
{
    Q_OBJECT

public:
    CMqttTopic();
    CMqttTopic( const CMqttTopic &other );
    CMqttTopic( const QString &topicName );
    CMqttTopic( const QString &topicName, const QByteArray & data );
    virtual ~CMqttTopic();

/** ***********************************************************************
 * @brief Методы получения содержимого свойств базового класса
 ** ***********************************************************************/
    inline const QString & topicName() const
    {
        return m_topicName;
    }
    inline const QByteArray data() const
    {
        return prepareData();
    }
    inline bool isValid() const
    {
        return m_valid;
    }

    inline const QString & mqttConnectionName() const
    {
        return m_mqttConnectionName;
    }
    void setMqttConnectionName( const QString & connName );


#ifdef MNT_TOPICS_TEST
/** ***********************************************************************
 * @fn getEtalonJson
 * @brief Возвращает эталонный json для тестирования
 ** ***********************************************************************/
public:
    virtual QByteArray getEtalonJson()
    {
        return QByteArray();
    }
#endif

/** ***********************************************************************
 * @fn initFromData
 * @brief Метод инициализации полей класса из принятого сообщения
 * @param const QByteArray & data - сырые данные для разбора
 * @return bool - результат разбора принятых данных
 ** ***********************************************************************/
    bool initFromData( const QByteArray & data );
/** ***********************************************************************
 * @fn buildMessage
 * @brief Сборка сообщения для отправки
 * @param bool retain = false
 ** ***********************************************************************/
    const CMqttMessage buildMessage(bool retain = false) const;

protected:
/** ***********************************************************************
 * @fn prepareData
 * @brief [virtual] Метод инициализации полей класса из принятого сообщения
 * В большинстве случаев, на выходе JSON
 * @return QByteArray
 ** ***********************************************************************/
    virtual const QByteArray prepareData() const;
    const QByteArray prepareFromInt( const int value ) const;

protected:
/** ***********************************************************************
 * @fn parseData
 * @brief [virtual] Метод разбора принятого сообщения
 * @param const QByteArray & data
 * @return bool
 ** ***********************************************************************/
    virtual bool parseData( const QByteArray & data );
/** ***********************************************************************
 * @fn parseToInt
 * @brief [static] Стандартный метод разбора сообщения вида { "Value": int }
 * @param const QByteArray & data - принятые данные
 * @param bool * res - указатель на флаг с результатом преобразования.
 * Допускается указывать значение nullptr
 * @return int
 ** ***********************************************************************/
    int parseToInt( const QByteArray & data, bool * res = nullptr );

    void printJsonParseErrorMessage( const QByteArray & data, const QJsonParseError & error ) const;
    void printEmptyJsonMessage() const;
    void printUnknownFieldsListMessage(const QStringList &filedsList) const;
    inline bool setValidState( bool valid )
    {
        m_valid = valid;
        return m_valid;
    }

signals:
/** ***********************************************************************
 * @fn messageReceived
 * @brief Сигнал о получении данных
 ** ***********************************************************************/
    void messageReceived();

private:
    QString m_topicName{};
    QByteArray m_data{};
    bool m_valid{ false };

    QString m_mqttConnectionName{};
};

#endif
