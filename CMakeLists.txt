cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(smarthome VERSION 1.0)

include_directories(
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src
)
file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/inc/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
add_executable(smarthome ${all_SRCS})
target_include_directories(smarthome PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(smarthome config++ paho-mqttpp3 paho-mqtt3as microhttpd pthread)

install(
    FILES ${PROJECT_SOURCE_DIR}/res/systemd/smarthome.service
    DESTINATION /etc/systemd/system/
)
install(
    TARGETS smarthome
    RUNTIME DESTINATION /usr/bin
)
