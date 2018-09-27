#include <queue>
#include "thread.h"
#include <sys/time.h>
#include <vector>
#include <memory>
#include <signal.h>


#ifndef CPP_STUFF_THREADS_MANAGER_H
#define CPP_STUFF_THREADS_MANAGER_H

const int RETURNED_FROM_JUMP = 1;

class ThreadsManager
{
private:
    unsigned int quantum_usecs;
    static std::queue<std::shared_ptr<Thread>> ready_queue;
    static std::shared_ptr<Thread> running_thread;
    static std::vector<std::shared_ptr<Thread>> all_threads;
    static void quantum_end_handler(int sig_number);
    struct itimerval timer;
    struct sigaction sa;


public:
    explicit ThreadsManager(unsigned int quantum_usecs);
    void spawn_thread(void (*f)(void), unsigned int thread_id);

};
#endif //CPP_STUFF_THREADS_MANAGER_H
