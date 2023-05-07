TARGET = sdk_test_units
DEFINES += MNT_TOPICS_TEST
unix {
    OBJECTS_DIR = .obj
    MOC_DIR     = .moc
    QMAKE_CXXFLAGS += -Wall -Werror -Wextra -pedantic
}

include(../app.pri)


QT += testlib

SOURCES = main.cpp \
            src/SdkTestUnits.cpp

HEADERS = src/SdkTestUnits.hpp

include(src/tests.pri)

buildTopicsList.commands = $$PWD/build_topics_list.sh $$PWD/.. Mqtt/Topics > topics_list.h

QMAKE_EXTRA_TARGETS += buildTopicsList
PRE_TARGETDEPS += buildTopicsList
QMAKE_CLEAN += buildTopicsList.target
include(../Mqtt.pri)
