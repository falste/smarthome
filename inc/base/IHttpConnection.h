
#ifndef IHTTPCONNECTION_H_
#define IHTTPCONNECTION_H_

#include <string>

class IPostMessageReceiver {
    public:
        virtual void onPostMessage(std::string key, std::string value) = 0;
}; // class IPostMessageReceiver

class IHttpConnection {
    public:
        virtual void registerPostMessageReceiver(IPostMessageReceiver* recv) = 0;
};

#endif // IHTTPCONNECTION_H_
