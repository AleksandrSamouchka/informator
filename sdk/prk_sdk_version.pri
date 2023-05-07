## SDK version

VERSION = 0.7.2
SDK_BUILD_NUM = $$system(cd $$PWD && git rev-parse --short HEAD)
DEFINES += SDK_VERSION='\\"$${VERSION}\\"'
DEFINES += SDK_REVISION='\\"$${SDK_BUILD_NUM}\\"'