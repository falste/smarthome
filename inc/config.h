
#ifndef CONFIG_H_
#define CONFIG_H_

#include <chrono>
#include <string>

using namespace std::literals::chrono_literals;

namespace cfg {

// PresenceDetector
const std::string routerIp = "192.168.178.1";
const std::string phoneIp =  "192.168.178.4";
constexpr uint8_t maxFailedPings = 30;
constexpr std::chrono::duration delay = 1s;

// HttpConnection
const std::string kHtmlFile =       "/smarthome/web/site.html";
const std::string kFaviconFile =    "/smarthome/web/favicon.ico";

const std::string kLocalIp = "192.168.178.3";
constexpr uint16_t kPort = 80;

// MqttConnection
const std::string kMqttBrokerAddress = "tcp://smarthome-mosquitto:1883";
const std::string kMqttTopicPrefix = "zigbee2mqtt";
const std::string kMqttClientName = "smarthome_client";

const std::string kPersistDir = "/smarthome/.persist";

} // namespace cfg

#endif // CONFIG_H_

