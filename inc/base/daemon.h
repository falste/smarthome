// https://github.com/ftraple/cpp-daemon-template

#ifndef DAEMON_H_
#define DAEMON_H_

#include <csignal>
#include <functional>

class Daemon {
   public:
    static Daemon& getInstance() {
        static Daemon instance;
        return instance;
    }

    void setReloadFunction(std::function<void()> func);

    bool isRunning();

   private:
    std::function<void()> m_reloadFunc;
    bool m_isRunning;
    bool m_reload;

    Daemon();
    Daemon(Daemon const&) = delete;
    void operator=(Daemon const&) = delete;

    void Reload();

    static void signalHandler(int signal);
};

#endif  // DAEMON_H_
