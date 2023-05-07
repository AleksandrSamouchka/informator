#include "CFormalizedMessage.h"


CFormalizedMessage::CFormalizedMessage( const int direction,
                                        const int serverId,
                                        const uint requestId,
                                        const uint messageId,
                                        const QString message,
                                        const qulonglong id,
                                        const qlonglong time,
                                        const int isProcessed,
                                        const int isReaded) :
    mDirection{ direction },
    mServerId{ serverId },
    mRequestId{ requestId },
    mMessageId{ messageId },
    mMessage{ message },
    mId{ id },
    mTime{ time },
    mIsProcessed{ isProcessed > 0 },
    mIsReaded{ isReaded > 0 }
{ }
