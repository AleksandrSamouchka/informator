QT += sql

HEADERS += \
    $$PWD/CKeepAliveHandler/CKeepAliveHandler.h \
    $$PWD/CKeepAliveHandler/CKeepAliveTopicMessage.h

SOURCES += \
    $$PWD/CKeepAliveHandler/CKeepAliveHandler.cpp \
    $$PWD/CKeepAliveHandler/CKeepAliveTopicMessage.cpp

DEFINES += USE_KEEP_ALIVE_HANDLER
