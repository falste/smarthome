
#include <chrono>

#include "log.h"
#include "Timer.h"

using namespace std::literals::chrono_literals;
using fiveMin = std::chrono::duration<int, std::ratio<360,1>>;
constexpr std::chrono::duration delay = 5s;

Timer::Timer(){
    hThread_ = std::thread(&Timer::trackTime, this);
}

Timer::~Timer(){}

void Timer::registerTimeUpdateReceiver(ITimeUpdateReceiver* recv){
    if (recv == nullptr) {
        Log(Level::Err, "Trying to register nullptr");
        return;
    }

    recv_.push_back(recv);
}

void Timer::trackTime() {
    while(true) {
//        Log(Level::Debug, "Five minute update");

        std::tm now = getTime();

        for (ITimeUpdateReceiver* recv : recv_) {
            recv->onFiveMinutes(now);
        }

        std::tm wakeupTime = now;
        wakeupTime.tm_min = wakeupTime.tm_min - wakeupTime.tm_min%5 + 5; // Round up to next 5 minutes
        wakeupTime.tm_sec = 0;
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&wakeupTime));
        std::this_thread::sleep_until(tp);
    }
}

std::tm Timer::getTime() {
    time_t t = time(0);
    return *localtime(&t);
}
