
#ifndef HTTPCONNECTION_H_
#define HTTPCONNECTION_H_

#include <sstream>
#include <vector>
#include <microhttpd.h>

#include "base/IHttpConnection.h"

class HttpConnection : public IHttpConnection {
    public: // Singleton pattern
        static HttpConnection& getInstance() {
            static HttpConnection instance;
            return instance;
        }

        HttpConnection(HttpConnection const&) = delete;
        void operator=(HttpConnection const&) = delete;

    public:
        void registerPostMessageReceiver(IPostMessageReceiver* recv);


    private:
        HttpConnection();
        ~HttpConnection();

        static int onConnection(void* cls, MHD_Connection* con, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls);
        static void onConnectionFinished(void* cls, MHD_Connection* con, void** con_cls, enum MHD_RequestTerminationCode code);
        static int processPostData (void *cls, enum MHD_ValueKind kind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size);

        static int sendDefaultResponse(MHD_Connection* connection, const std::string& data);
        static int sendRedirectResponse(MHD_Connection* connection, const std::string& location);
        static bool readFile(std::string path, std::string& data);

    private:
        MHD_Daemon* httpDaemon_ = nullptr;

        inline static std::string keyPem_;
        inline static std::string certPem_;

        inline static std::string html_;
        inline static std::string favicon_;

        inline static std::vector<IPostMessageReceiver*> callbacks_;
};

#endif // HTTPCONNECTION_H_

