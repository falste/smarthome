# smarthome
A custom smart home project with a web interface with the goal of communicating with zigbee devices.

As part of its automation, the program responds to certain day times as well as a device in the LAN leaving or joining the network.

Zigbee devices are currently hardcoded in Controller.h. (See TODOs)

# Build instructions
Install the paho library for C++: https://github.com/eclipse/paho.mqtt.cpp#building-from-source

Then, run from within the project folder:
```
sudo apt-get install cmake libconfig++-dev libmicrohttpd-dev
mkdir build
cd build
cmake ..
```

To build this project, run from build folder:
```
make
```

To install this project, run from build folder:
```
sudo make install
```

If you are working on the code, something like this works well, too:
```
sudo make install && sudo systemctl restart smarthome.service && journalctl -fu smarthome.service -n50 -p7
```

# TODO
- Move certain hardcoded variables to a config file, like number of tries and address to ping for presence detection.
- Move html file to a folder on install and link to that one in the code.
- Handle remaining TODOs in code
- Handle zigbee devices restarting themselves
- Move zigbee devices to a config file

# Contributing
I'd be happy to see this being used and even more happy if people were to contribute. Feel free to contact me!

