include(CSettings.pri)

QT += sql

HEADERS += \
    $$PWD/CDatabaseHandler/CDatabaseHandler.h \
    $$PWD/CDatabaseHandler/CDatabaseConnectionSettings.h \
    $$PWD/CDatabaseHandler/DatabaseContent.h

SOURCES += \
    $$PWD/CDatabaseHandler/CDatabaseHandler.cpp \
    $$PWD/CDatabaseHandler/CDatabaseConnectionSettings.cpp
