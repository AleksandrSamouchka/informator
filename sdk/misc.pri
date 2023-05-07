QT += sql

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/misc/informator/cinformatormisc.h \
    $$PWD/misc/informator/informator_defines.h \
    $$PWD/misc/sound_templates/soundtemplates.h \
    $$PWD/misc/routes/routes.h \
    $$PWD/misc/routes/routes_file.h

SOURCES += \
    $$PWD/misc/informator/cinformatormisc.cpp \
    $$PWD/misc/informator/informator_defines.cpp \
    $$PWD/misc/sound_templates/soundtemplates.cpp \
    $$PWD/misc/routes/routes_file.cpp
