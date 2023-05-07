#ifndef CFORMALIZEDMESSAGE_H
#define CFORMALIZEDMESSAGE_H

/** ***********************************************************************
 * @class CFormalizedMessage
 * @brief Структура формализованного сообщения
 * @author Мальнев А
 * @date 20-02-2019
 ** ***********************************************************************/

#include <QtCore/QObject>

/** ***********************************************************************
 * @class CFormalizedMessage
 ** ***********************************************************************/
class CFormalizedMessage
{
public:
    enum EDirection
    {
        ToServer = 0,
        FromServer,
    };

public:
    int mDirection{ 0 };
    int mServerId{ 0 };
    uint mRequestId{ 0 };
    uint mMessageId{ 0 };
    QString mMessage{};

    qulonglong mId{ 0 };
    qlonglong mTime{ 0 };
    bool mIsProcessed{ false };
    bool mIsReaded{ false };

public:
    CFormalizedMessage() = default;
    CFormalizedMessage( const CFormalizedMessage & other ) = default;

    CFormalizedMessage( const int direction,
                        const int serverId,
                        const uint requestId,
                        const uint messageId,
                        const QString message,
                        const qulonglong id,
                        const qlonglong time,
                        const int isProcessed,
                        const int isReaded );

    inline bool isValid() const
    {
        return mTime != 0;
    }


};

#endif // CFORMALIZEDMESSAGE_H
