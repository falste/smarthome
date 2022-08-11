cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

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
    TARGETS smarthome
    RUNTIME DESTINATION /usr/bin
)
install(
    FILES ${PROJECT_SOURCE_DIR}/res/systemd/smarthome.service
    DESTINATION /etc/systemd/system/
)
install(
    FILES ${PROJECT_SOURCE_DIR}/res/web/site.html ${PROJECT_SOURCE_DIR}/res/web/favicon.ico
    DESTINATION /opt/smarthome/web/
)
