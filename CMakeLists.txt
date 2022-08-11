cmake_minimum_required(VERSION 3.16)

project(smarthome VERSION 1.0)

include_directories(
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src
)
file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/inc/*.h"
    "${PROJECT_SOURCE_DIR}/inc/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)
add_executable(smarthome ${all_SRCS})
target_include_directories(smarthome PUBLIC "${PROJECT_BINARY_DIR}")

configure_file(${PROJECT_SOURCE_DIR}/inc/SmarthomeConfig.h.in SmarthomeConfig.h)

target_link_libraries(smarthome config++)

install(
    FILES ${PROJECT_SOURCE_DIR}/res/systemd/smarthome.service
    DESTINATION /etc/systemd/system/
)
install(
    TARGETS smarthome
    RUNTIME DESTINATION /usr/bin
)
