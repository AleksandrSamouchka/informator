## SDK

CONFIG(SDK_PKG) {
    message( "Building with mntconti-sdk-dev package" )
    QT_CONFIG -= no-pkg-config
    SDK_DIR = /usr/share/libsdk
    CONFIG += link_pkgconfig
    PKGCONFIG += sdk
} else {
    message( "Building with local SDK" )
    SDK_DIR = $$PWD/../sdk

    sources_bak = $$SOURCES
    headers_bak = $$HEADERS

    include(common_sdk.pri)

    SOURCES = $$sources_bak
    HEADERS = $$headers_bak

    LIBS += -L$$SDK_DIR
}

LIBS += -lsdk

include($${SDK_DIR}/prk_sdk_version.pri)