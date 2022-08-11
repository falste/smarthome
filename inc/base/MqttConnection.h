
#ifndef MQTTCONNECTION_H_
#define MQTTCONNECTION_H_

#include <vector>

#include "mqtt/async_client.h"

#include "base/IMqttConnection.h"

class MqttConnection : public IMqttConnection, public mqtt::callback, public mqtt::iaction_listener {
    public: // Singleton pattern
        static MqttConnection& getInstance() {
            static MqttConnection instance;
            return instance;
        }

        MqttConnection(MqttConnection const&) = delete;
        void operator=(MqttConnection const&) = delete;

    public:
        void registerMessageReceiver(IMqttMessageReceiver* recv, std::string friendlyName) override;
        void send(std::string friendlyName, std::string command, std::string payload) override;
        void send(std::string friendlyName, std::string command, std::string key, std::string value) override;

    private:
        MqttConnection();
        ~MqttConnection();

    private: // Paho-MQTT interaction
        void connected(const std::string& cause) override;
        void message_arrived(mqtt::const_message_ptr msg) override;
        void on_failure(const mqtt::token& tok) override;
        void on_success(const mqtt::token& tok) override;

    private:
        std::unique_ptr<mqtt::async_client> client_;
        std::map<std::string, IMqttMessageReceiver*> messageReceivers_;

}; // class IMqttConnection

#endif // IMQTTCONNECTION_H_
