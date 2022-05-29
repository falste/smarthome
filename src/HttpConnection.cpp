
#include <string>
#include <cstring>

#include <fstream>

#include "log.h"
#include "HttpConnection.h"

/*
 * Constants
 */

const std::string kServerKeyFile = "/etc/apache2/certificate/apache.key.pem";
const std::string kServerCertFile = "/etc/apache2/certificate/apache.cert.pem";

const std::string kHtmlFile = "/home/pi/smarthome/res/web/site.html";
const std::string kFaviconFile = "/home/pi/smarthome/res/web/favicon.ico";

const std::string kLocalIp = "https://192.168.178.3:8081";

constexpr uint16_t kPort = 8081;
constexpr uint16_t kPostBufferSize = 1024;

/*
 * Definitions
 */

enum class ConnectionType { kGet, kPost };

struct ConData
{
    ConnectionType type;
    uint8_t iteration;
    MHD_PostProcessor *postprocessor; 
};

/*
 * Functions
 */

HttpConnection::HttpConnection() {
    if (!readFile(kHtmlFile, html_) ||
        !readFile(kServerKeyFile, keyPem_) ||
        !readFile(kServerCertFile, certPem_) ||
        !readFile(kFaviconFile, favicon_)) {
        return;
    }

    httpDaemon_ = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_SSL | MHD_USE_ERROR_LOG,
        kPort,
        nullptr,
        nullptr,
        &onConnection,
        nullptr,
        MHD_OPTION_NOTIFY_COMPLETED, &onConnectionFinished, nullptr,
        MHD_OPTION_HTTPS_MEM_KEY, keyPem_.c_str(),
        MHD_OPTION_HTTPS_MEM_CERT, certPem_.c_str(),
        MHD_OPTION_END);

    if (httpDaemon_ == nullptr) {
        Log(Level::Err, "Failed to start HTTP daemon");
    } else {
        Log(Level::Info, "HTTP daemon started successfully");
    }
}

HttpConnection::~HttpConnection() {
    if (httpDaemon_ != nullptr) {
        MHD_stop_daemon(httpDaemon_);
    }
}

void HttpConnection::registerPostMessageReceiver(IPostMessageReceiver* recv) {
    if (recv != nullptr) {
        callbacks_.push_back(recv);
    }
}

int HttpConnection::onConnection(void* cls,
    MHD_Connection* connection,
    const char* url,
    const char* method,
    const char* version,
    const char* upload_data,
    size_t* upload_data_size,
    void** con_cls) {

    /*
    if (*con_cls == nullptr) {
        Log(Level::Debug, "HTTP: 0 - " + std::string(method) + " " + std::string(url));
    } else {
        ConData* conData = reinterpret_cast<ConData*>(*con_cls);
        conData->iteration++;
        Log(Level::Debug, "HTTP: " + std::to_string(conData->iteration) + " - " + std::string(method) + " " + std::string(url));
    }
    */

    // Initial setup
    if(*con_cls == nullptr) {
        *con_cls = malloc(sizeof(ConData));
        ConData* conData = reinterpret_cast<ConData*>(*con_cls);
   
        if (conData == nullptr) {
            Log(Level::Err, "Failed to malloc conData");
            return MHD_NO;
        }

        if (strcmp(method, MHD_HTTP_METHOD_GET) == 0)  {
            conData->type = ConnectionType::kGet;

        } else if (strcmp(method, MHD_HTTP_METHOD_POST) == 0) {
            conData->type = ConnectionType::kPost;
            conData->postprocessor = MHD_create_post_processor(connection, kPostBufferSize, &processPostData, *con_cls);
    
            if (conData->postprocessor == nullptr) {
                free(conData);
                Log(Level::Err, "Failed to create POST processor, refusing connection");
                return MHD_NO;
            }
        }

        return MHD_YES;
    }

    ConData* conData = reinterpret_cast<ConData*>(*con_cls);

    if (strcmp(method, "POST") == 0) {
        ConData *conData = reinterpret_cast<ConData*>(*con_cls);

        if (*upload_data_size != 0) {
            if (MHD_post_process(conData->postprocessor, upload_data, *upload_data_size) != MHD_YES) {
                free(conData);
                Log(Level::Err, "POST processing failed, refusing connection");
                return MHD_NO;
            }
            *upload_data_size = 0;
            return MHD_YES;
        }

        return sendRedirectResponse(connection, kLocalIp);
    }

    if (strcmp(method, "GET") == 0 && strcmp(url, "/favicon.ico") == 0) {
        return sendDefaultResponse(connection, favicon_);
    }

    return sendDefaultResponse(connection, html_);
}

void HttpConnection::onConnectionFinished(void *cls, MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode code) {
    ConData* conData = reinterpret_cast<ConData*>(*con_cls);
    if (conData == nullptr) {
        return;
    }

    if (conData->type == ConnectionType::kPost) {
        MHD_destroy_post_processor(conData->postprocessor);
    }

    free(conData);
    *con_cls = nullptr;
}

int HttpConnection::processPostData (void *cls, enum MHD_ValueKind kind, const char *key,
              const char *filename, const char *content_type,
              const char *transfer_encoding, const char *data,
	      uint64_t off, size_t size)
{
    if (size == 0) {
        return MHD_YES;
    }

    for(IPostMessageReceiver* recv : callbacks_) {
        recv->onPostMessage(std::string(key), std::string(data));
    }

    return MHD_YES;
}

int HttpConnection::sendDefaultResponse(MHD_Connection* connection, const std::string& data) {
    MHD_Response* response;
    response = MHD_create_response_from_buffer(
        data.length(), (void*) data.c_str(), MHD_RESPMEM_PERSISTENT
    );
    if (response == nullptr) {
        Log(Level::Err, "Failed to create HTTP response, refusing connection");
        return MHD_NO;
    }

    int ret;
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    if (ret != MHD_YES) {
        Log(Level::Err, "Failed to queue HTTP response, refusing connection");
        return MHD_NO;
    }
    return MHD_YES;
}

int HttpConnection::sendRedirectResponse(MHD_Connection* connection, const std::string& location) {
    MHD_Response *response;
    response = MHD_create_response_from_buffer(
        0, nullptr, MHD_RESPMEM_MUST_COPY
    );
    if (response == nullptr) {
        Log(Level::Err, "Failed to create HTTP response, refusing connection");
        return MHD_NO;
    }
    int ret;
    ret = MHD_add_response_header(response, "Location", location.c_str());
    if (ret != MHD_YES) {
        Log(Level::Err, "Failed to create response header, refusing connection");
        return MHD_NO;
    }

    ret = MHD_queue_response(connection, MHD_HTTP_FOUND, response);
    MHD_destroy_response(response);

    if (ret != MHD_YES) {
        Log(Level::Err, "Failed to queue HTTP response, refusing connection");
        return MHD_NO;
    }
    return MHD_YES;
}

bool HttpConnection::readFile(std::string path, std::string& data) {
    std::ifstream fileStream = std::ifstream(path);
    if (!fileStream.is_open()) {
        Log(Level::Err, "Failed to open file " + path);
        return false;
    }

    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    data = stringStream.str();

    return true;
}

