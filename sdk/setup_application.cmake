include(${MNTCONTI_SDK_DIR}/common_sdk.cmake)

setup_by_qmake_project()
add_definitions(-DAPP_VERSION="${VERSION}" -DAPP_REVISION="${REVISION}")

find_mntconti_sdk()
include_directories(${MNTCONTI_SDK_DIR})

if (TARGET_PREFIX)
    set(TARGET ${TARGET_PREFIX}_${CMAKE_PROJECT_NAME})
else()
    set(TARGET ${CMAKE_PROJECT_NAME})
endif ()

add_executable(${TARGET} ${SOURCES} ${HEADERS} ${RESOURCES})
target_link_libraries(${TARGET} ${LIBS} ${MNTCONTI_SDK})
