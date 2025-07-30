FROM arm64v8/ubuntu:18.04 AS build_env

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        gcc-9 g++-9 \
        ca-certificates \
        curl \
        cmake \
        git \
        iputils-ping \
        libconfig++-dev \
        libmicrohttpd-dev \
        libssl-dev && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 90 > /dev/null

# Install paho C and C++ library
WORKDIR /paho
RUN git clone https://github.com/eclipse/paho.mqtt.c.git && \
    cd paho.mqtt.c && \
    git checkout v1.3.8 && \
    cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON && \
    cmake --build build/ --target install && \
    cmake --build build/ --target paho-mqtt3as-static -- -j1 && \
    ldconfig && \
    cd .. && \
    rm -r paho.mqtt.c

RUN git clone https://github.com/eclipse/paho.mqtt.cpp.git && \
    cd paho.mqtt.cpp && \
    git checkout v1.2.0 && \
    cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON -DCMAKE_PREFIX_PATH=/usr/local/lib/cmake/eclipse-paho-mqtt-c && \
    cmake --build build/ --target install -- -j1 && \
    ldconfig && \
    cd .. && \
    rm -r paho.mqtt.cpp

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
