#include <queue>
#include "thread.h"
#include <sys/time.h>


#ifndef CPP_STUFF_THREADS_MANAGER_H
#define CPP_STUFF_THREADS_MANAGER_H

const int RETURNED_FROM_JUMP = 1;

class ThreadsManager
{
private:
    unsigned int quantum_usecs;
    static std::queue<std::unique_ptr<Thread>> ready_list;
    static std::unique_ptr<Thread> running_thread;
    static void quantum_end_handler(int sig_number);
    struct itimerval timer;


public:
    explicit ThreadsManager(unsigned int quantum_usecs);

};
#endif //CPP_STUFF_THREADS_MANAGER_H
