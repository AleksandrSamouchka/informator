cmake_minimum_required(VERSION 3.7)
project(sdk)

include(common_sdk.cmake)

setup_by_qmake_project(FALSE)
#  это хитрая тонкость qmake отчего-то реагирует нармально, а вот cmake приходится объяснять
set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Mqtt/Topics/Audio/CAudioMuteTopic.cpp PROPERTY SKIP_AUTOMOC ON)
# это придётся делать в каждом app никуда не денешься: имена промежуточных переменных не совпадают в sdk и приложениях, ну и гори оно огнём
add_definitions(-DSDK_VERSION="${VERSION}" -DSDK_REVISION="${REVISION}")

#set(MNTCONTI_SDK_LIB_DIR ${CMAKE_BINARY_DIR}/../lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MNTCONTI_SDK_LIB_DIR})

add_library(sdk SHARED ${SOURCES} ${HEADERS} sdk.h ${RESOURCES})
target_link_libraries(sdk ${LIBS})
set_property(TARGET sdk PROPERTY AUTOMOC ON)
