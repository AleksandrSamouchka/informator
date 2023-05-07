## Stack trace

LIBS += -rdynamic

HEADERS += \
    $$PWD/CrashHandler/CrashHandler.h \
    $$PWD/CrashHandler/StackTrace.h

SOURCES += \
    $$PWD/CrashHandler/CrashHandler.cpp \
    $$PWD/CrashHandler/StackTrace.cpp
