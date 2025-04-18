# smarthome
![Build](https://github.com/falste/smarthome/actions/workflows/build.yml/badge.svg)

A custom smart home project with a web interface with the goal of communicating with zigbee devices.

As part of its automation, the program responds to certain day times as well as a device in the LAN leaving or joining the network.

Zigbee devices are intentionally hardcoded in Controller.cpp and Controller.h. This way, if you want to use this project, you only need to know C++ and not some obscure configuration ruleset like with many other smart home applications. Just fork the project and make it your own!

# Build instructions

```
docker build .
```

# docker-compose

```
version: "2.1"
services:
  smarthome:
    image: ghcr.io/falste/smarthome:latest
    container_name: smarthome
    ports:
      - 8081:8081
```
