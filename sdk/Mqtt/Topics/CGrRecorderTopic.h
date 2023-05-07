#ifndef CGRRECORDERTOPIC_H
#define CGRRECORDERTOPIC_H

/** ***********************************************************************
 * @name CGrRecorderTopic
 * @brief Заготовка для группы топиков RECORDER/
 * @author Мальнев А
 * @date 20-01-2020
 ** ***********************************************************************/

#include <Mqtt/CMqttTopic.h>

class CGrRecorderTopic : public CMqttTopic
{
#ifdef MNT_TOPICS_TEST
    Q_OBJECT
public:
    CGrRecorderTopic() : CMqttTopic() {}
#endif
public:
    CGrRecorderTopic( const QString &topicName );
    ~CGrRecorderTopic();
};

#endif // CGRRECORDERTOPIC_H
