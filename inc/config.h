
#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>

namespace cfg {

// PresenceDetector
const std::string routerIp = "192.168.178.1";
const std::string phoneIp =  "192.168.178.4";
constexpr uint8_t maxFailedPings = 5;

// HttpConnection
const std::string kServerKeyFile =  "/smarthome/web/cert/server.key.pem";
const std::string kServerCertFile = "/smarthome/web/cert/server.cert.pem";
const std::string kHtmlFile =       "/smarthome/web/site.html";
const std::string kFaviconFile =    "/smarthome/web/favicon.ico";

const std::string kLocalIp = "192.168.178.3";
constexpr uint16_t kPort = 8081;

// MqttConnection
const std::string kMqttBrokerAddress = "tcp://mosquitto:1883";
const std::string kMqttTopicPrefix = "zigbee2mqtt";
const std::string kMqttClientName = "smarthome_client";

const std::string kPersistDir = "/smarthome/.persist";

} // namespace cfg

#endif // CONFIG_H_

