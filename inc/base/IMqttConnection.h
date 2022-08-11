
#ifndef IMQTTCONNECTION_H_
#define IMQTTCONNECTION_H_

#include <string>

class IMqttMessageReceiver {
    public:
        virtual void onMqttMessage(std::string command, std::string payload) = 0;
}; // class IMqttMessageReceiver

class IMqttConnection {
    public:
        virtual ~IMqttConnection() = default;

        virtual void registerMessageReceiver(IMqttMessageReceiver* recv, std::string friendlyName) = 0;
        virtual void send(std::string friendlyName, std::string command, std::string payload) = 0;
        virtual void send(std::string friendlyName, std::string command, std::string key, std::string value) = 0;

}; // class IMqttConnection

#endif // IMQTTCONNECTION_H_
