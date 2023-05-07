TEMPLATE = app
TARGET = mntconti_$$TARGET
QT -= gui
QT -= network
QT -= multimedia

isEmpty(VERSION):VERSION = 1.0.0
BUILD_NUM = $$system(cd $$_PRO_FILE_PWD_ && git rev-parse --short HEAD)
DEFINES += APP_VERSION=\\\"$${VERSION}\\\"
DEFINES += APP_REVISION=\\\"$${BUILD_NUM}\\\"

CONFIG(debug, debug|release) {
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    message("Enter to debug, release with app.pri")

    sources_bak = $$SOURCES
    headers_bak = $$HEADERS

    include(common_sdk.pri)

    SOURCES = $$sources_bak
    HEADERS = $$headers_bak

    # lib SDK path
    sdk_dir=$$absolute_path(sdk, $$OUT_PWD)
    !exists($$sdk_dir):sdk_dir = $$absolute_path(../sdk, $$OUT_PWD)
    !exists($$sdk_dir):sdk_dir = $$absolute_path(../../sdk, $$OUT_PWD)
    !exists($$sdk_dir):sdk_dir = $$absolute_path(../../../sdk, $$OUT_PWD)
    !exists($$sdk_dir):error("SDK library path is not found!!!")
    exists($$sdk_dir):message("Found sdk at $$sdk_dir")

    win32:CONFIG(release, debug|release): LIBS += -L$$sdk_dir/release
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$sdk_dir/debug
    else:unix: LIBS += -L$$sdk_dir
}

yocto {
    CONFIG(debian, debian|yocto) {
        QT_CONFIG -= no-pkg-config
        CONFIG += link_pkgconfig
        PKGCONFIG += sdk
    }
}

!yocto {
    CONFIG(debug, debug|release) {
        message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	message("Enter to debug, release pkgs configs")
        QT_CONFIG -= no-pkg-config
        CONFIG += link_pkgconfig
        PKGCONFIG += sdk
    }
}

LIBS += -lsdk


unix {
    INSTALLS += target
 	!isEmpty(INSTALL_DIR){
    	target.path=$${INSTALL_DIR}/bin
    }
    QMAKE_CLEAN += $(TARGET) $(QMAKE_TARGET)
}
