
#include <chrono>
#include <string>

#include "log.h"

#include "MqttConnection.h"

const std::string kAddress = "tcp://localhost:1883";
const std::string kClientId = "smarthome_client";
const std::string kPersistDir = "./persist";

const std::string kTopicPrefix = "zigbee2mqtt";

const int  kQOS = 1;
const auto kTimeout = std::chrono::seconds(10);

const uint16_t kMaxLogLength = 150;

MqttConnection::MqttConnection() {
	client_ = std::make_unique<mqtt::async_client>(kAddress, kClientId, kPersistDir);

	auto options = mqtt::connect_options_builder()
		.clean_session()
		.finalize();

    client_->set_callback(*this);

	try {
		mqtt::token_ptr conntok = client_->connect(options);
		conntok->wait();
	} catch (const mqtt::exception& e) {
		Log(Level::Err, e.what());
	}
}

MqttConnection::~MqttConnection() {
    try {
		// Disconnect
		client_->disconnect()->wait();
	} catch (const mqtt::exception& e) {
		Log(Level::Err, e.what());
	}
}

void MqttConnection::registerMessageReceiver(IMqttMessageReceiver* recv, std::string friendlyName) {
    if (recv == nullptr) {
        Log(Level::Err, "Trying to register nullptr as mqtt message receiver");
        return;
    }
    if (friendlyName.length() < 1) {
        Log(Level::Err, "Trying to register device with bad friendlyName");
        return;
    }

    this->messageReceivers_.emplace(friendlyName, recv);
}

void MqttConnection::send(std::string friendlyName, std::string command, std::string payload) {
    std::string topic = kTopicPrefix + "/" + friendlyName + "/" + command;

    try {
		mqtt::message_ptr msg = mqtt::make_message(topic, payload);
		msg->set_qos(kQOS);
		client_->publish(msg)->wait_for(kTimeout);
	} catch (const mqtt::exception& e) {
		Log(Level::Err, e.what());
	}
}

void MqttConnection::send(std::string friendlyName, std::string command, std::string key, std::string value) {
    send(friendlyName, command, "{" + key + ": " + value + "}");
}

void MqttConnection::connected(const std::string& cause) {
    std::string topicSub = kTopicPrefix + "/#";
    Log(Level::Info, "Subscribing to MQTT topic '" + topicSub + "' using QoS" + std::to_string(kQOS));
    client_->subscribe(topicSub, kQOS, nullptr, *this);
}

void MqttConnection::message_arrived(mqtt::const_message_ptr msg) {
    std::string logMsg = msg->get_topic() + ": " + msg->to_string();
    if (logMsg.length() > kMaxLogLength-3) { // -3 to account for added dots
        Log(Level::Debug, logMsg.substr(0, kMaxLogLength-3) + "...");
    } else {
        Log(Level::Debug, logMsg);
    }

    std::string payload = msg->to_string();
    std::string topic = msg->get_topic();
    std::string delimiter = "/";

    // Remove leading '/', if there is one
    if (topic[0] == '/') {
        topic.erase(0, 1);
    }

    /*
     * The topic is expected to be of the format kTopicPrefix/friendlyName/command
     */
    //Get and remove kTopicPrefix from topic
    std::string token = topic.substr(0, topic.find(delimiter));
    topic.erase(0, token.length()+1);

    if (token != kTopicPrefix) {
        Log(Level::Warn, "Topic prefix not as expected, skipping message");
        return;
    }

    // Get and remove friendlyName from topic
    token = topic.substr(0, topic.find(delimiter));
    topic.erase(0, token.length()+1);

    std::string friendlyName = token;

    // Get command from topic
    std::string command = topic;

    auto iterator = messageReceivers_.find(friendlyName);
    if (iterator != messageReceivers_.end()) {
        iterator->second->onMqttMessage(command, payload);
    }
}

void MqttConnection::on_success(const mqtt::token& tok) {
}

void MqttConnection::on_failure(const mqtt::token& tok) {
    Log(Level::Warn, "Mqtt failure");
}
