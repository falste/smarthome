FROM arm32v7/ubuntu:18.04 AS build_env

RUN apt-get update && \
    apt install -y --no-install-recommends build-essential ca-certificates cmake git libconfig++-dev libmicrohttpd-dev libssl-dev > /dev/null

# Install paho C and C++ library
WORKDIR /paho
RUN git clone https://github.com/eclipse/paho.mqtt.c.git && \
    cd /paho/paho.mqtt.c && \
    git checkout v1.3.8 && \
    cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON && \
    cmake --build build/ --target install && \
    ldconfig && \
    cd .. && \
    rm -r /paho/paho.mqtt.c

RUN git clone https://github.com/eclipse/paho.mqtt.cpp && \
    cd /paho/paho.mqtt.cpp && \
    cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON && \
    cmake --build build/ --target install && \
    ldconfig && \
    cd .. && \
    rm -r /paho/paho.mqtt.cpp

# Install smarthome software
COPY . /smarthome_build
WORKDIR /smarthome_build/build
RUN cmake .. && \
    make && \
    mkdir /smarthome && \
    mv smarthome /smarthome && \
    mv ../res/web/ /smarthome/web/ && \
    cd /smarthome && \
    rm -r /smarthome_build

WORKDIR /smarthome
CMD ["./smarthome"]
