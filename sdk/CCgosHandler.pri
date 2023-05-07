INCLUDEPATH += $$PWD/CCgosHandler
DEFINES += HAS_CCGOSHANDLER

HEADERS += \
    $$PWD/CCgosHandler/CCgosHandler.h \
    $$PWD/CCgosHandler/AmpliferManager.h

SOURCES += \
    $$PWD/CCgosHandler/CCgosHandler.cpp \
    $$PWD/CCgosHandler/AmpliferManager.cpp

contains(DEFINES, HAS_CCGOSHANDLER) {
    LIBS+= -lcgos
}
