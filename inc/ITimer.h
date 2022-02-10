
#ifndef ITIMER_H_
#define ITIMER_H_

#include <ctime>

class ITimeUpdateReceiver {
    public:
        virtual void onFiveMinutes(tm time) = 0;
};

class ITimer {
    public:
        virtual void registerTimeUpdateReceiver(ITimeUpdateReceiver* recv) = 0;
        virtual tm getTime() = 0;

};

#endif // ITIMER_H_
