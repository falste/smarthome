# smarthome
![Build](https://github.com/falste/smarthome/actions/workflows/build.yml/badge.svg)

A custom smart home project with a web interface with the goal of communicating with zigbee devices.

As part of its automation, the program responds to certain day times as well as a device in the LAN leaving or joining the network.

Zigbee devices are intentionally hardcoded in Controller.cpp and Controller.h. This way, if you want to use this project, you only need to know C++ and not some obscure configuration ruleset like with many other smart home applications. Just fork the project and make it your own!

# Build instructions
Install the paho library for C++ with SSL support: https://github.com/eclipse/paho.mqtt.cpp#building-from-source

Then, run from within the project folder:
```
sudo apt-get install cmake libconfig++-dev libmicrohttpd-dev
cmake -B ./build
```

To install this project, run:
```
sudo cmake --build ./build --target install
```

Do not forget to provide a key and certificate file to:
```
/opt/smarthome/web/certificate/server.key.pem
/opt/smarthome/web/certificate/server.cert.pem
```

If you are working on the code, something like this works well, too:
```
sudo cmake --build ./build --target install && sudo systemctl restart smarthome.service && journalctl -fu smarthome.service -n50 -p7
```

# Contributing
I'd be happy to see this being used and even more happy if people were to contribute. Feel free to contact me!

