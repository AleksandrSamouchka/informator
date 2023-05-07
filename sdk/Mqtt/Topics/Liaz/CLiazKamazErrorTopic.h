#ifndef CLIAZKAMAZERRORTOPIC_H
#define CLIAZKAMAZERRORTOPIC_H

#include <Mqtt/Topics/CGrLiazTopic.h>

/** ***********************************************************************
 * @class CLiazKamazErrorTopic
 * @brief Список ошибок КАМАЗ
 ** ***********************************************************************/
class CLiazKamazErrorTopic : public CGrLiazTopic
{
public:
    explicit CLiazKamazErrorTopic();

};

#endif // CLIAZKAMAZERRORTOPIC_H
