#include <queue>
#include "thread.h"
#include <sys/time.h>
#include <vector>
#include <memory>
#include <csignal>


#ifndef CPP_STUFF_THREADS_MANAGER_H
#define CPP_STUFF_THREADS_MANAGER_H

const int RETURNED_FROM_JUMP = 1;

class Timer
{
private:
    struct itimerval timer;
    unsigned int timer_usecs;

public:
    void set_timer(unsigned int timer_usecs);
    void start_timer();
};


class ThreadsManager
{
private:
    static std::queue<std::shared_ptr<Thread>> ready_queue;
    static std::shared_ptr<Thread> running_thread;
    static std::vector<std::shared_ptr<Thread>> all_threads;
    static struct sigaction sa;
    static Timer timer;


public:
    explicit ThreadsManager(unsigned int quantum_usecs);
    void spawn_thread(void (*f)(), unsigned int thread_id);
    void terminate_thread(unsigned int thread_id);
    static void quantum_end_handler(int sig_number);


};
#endif //CPP_STUFF_THREADS_MANAGER_H
