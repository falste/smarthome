
#ifndef PRESENCEDETECTOR_H_
#define PRESENCEDETECTOR_H_

#include <thread>
#include <vector>

#include "IPresenceDetector.h"

class PresenceDetector : public IPresenceDetector {
    public: // Singleton pattern
        static PresenceDetector& getInstance() {
            static PresenceDetector instance;
            return instance;
        }

        PresenceDetector(PresenceDetector const&) = delete;
        void operator=(PresenceDetector const&) = delete;

    public:
        void registerPresenceReceiver(IPresenceReceiver* recv);
        PresenceState getPresence();

    private:
        PresenceDetector();
        ~PresenceDetector();

        void detect();

    private:
        std::vector<IPresenceReceiver*> recv_;

        std::thread hThread_;
        PresenceState presence_ = PresenceState::Unknown;
};

#endif // PRESENCEDETECTOR_H_
