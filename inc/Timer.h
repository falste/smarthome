

#ifndef TIMER_H_
#define TIMER_H_

#include <thread>
#include <vector>

#include "ITimer.h"

class Timer : public ITimer {
    public: // Singleton pattern
        static Timer& getInstance() {
            static Timer instance;
            return instance;
        }

        Timer(Timer const&) = delete;
        void operator=(Timer const&) = delete;

    public:
        void registerTimeUpdateReceiver(ITimeUpdateReceiver* recv);
        std::tm getTime();

    private:
        Timer();
        ~Timer();

        void trackTime();

    private:
        std::vector<ITimeUpdateReceiver*> recv_;

        std::thread hThread_;
};

#endif // TIMER_H_
