
#ifndef IPRESENCEDETECTOR_H_
#define IPRESENCEDETECTOR_H_

enum class PresenceState { Unknown, Present, Away };

class IPresenceReceiver {
    public:
        virtual void onPresenceChanged(PresenceState presence) = 0;
};

class IPresenceDetector {
    public:
        virtual void registerPresenceReceiver(IPresenceReceiver* recv) = 0;
        virtual PresenceState getPresence() = 0;

};

#endif // IPRESENCEDETECTOR_H_
