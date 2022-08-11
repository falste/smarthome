FROM arm32v7/ubuntu:18.04 AS build_env
COPY . /smarthome
RUN apt-get update
RUN apt install -y --no-install-recommends build-essential ca-certificates cmake libconfig++-dev libmicrohttpd-dev libssl-dev git > /dev/null

# Install paho C and C++ library
WORKDIR /paho
RUN git clone https://github.com/eclipse/paho.mqtt.c.git
WORKDIR /paho/paho.mqtt.c
RUN git checkout v1.3.8
RUN cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
RUN cmake --build build/ --target install
RUN ldconfig

WORKDIR /paho
RUN git clone https://github.com/eclipse/paho.mqtt.cpp
WORKDIR /paho/paho.mqtt.cpp
RUN cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON
RUN cmake --build build/ --target install
RUN ldconfig

# Install smarthome software
WORKDIR /smarthome/build
RUN cmake ..
RUN make

#FROM ubuntu:18.04 AS runtime_env
#COPY --from=build_env /smarthome/build/smarthome /smarthome/smarthome

#RUN apt-get update
#RUN apt install -y --no-install-recommends libconfig++-dev libmicrohttpd-dev libssl-dev > /dev/null

#WORKDIR /smarthome
CMD ["./smarthome"]
