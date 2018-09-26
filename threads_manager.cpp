#include "threads_manager.h"
#include <iostream>
#include <csignal>
#include <stdexcept>
#include <iostream>

std::unique_ptr<Thread> ThreadsManager::running_thread(new Thread(0));
std::queue<std::unique_ptr<Thread>> ThreadsManager::ready_list;

void ThreadsManager::quantum_end_handler(int sig_number)
{
    std::cout << "DING DONG!!" << std::endl;

    if(setjmp(running_thread -> get_buffer()) == RETURNED_FROM_JUMP)
    {
        return;
    }
    ready_list.push(std::move(running_thread));
    running_thread = std::move(ready_list.front());
    ready_list.pop();

    longjmp(running_thread -> get_buffer(), RETURNED_FROM_JUMP);

}

ThreadsManager::ThreadsManager(unsigned int quantum_usecs): quantum_usecs(quantum_usecs)
{
    if(setjmp(running_thread -> get_buffer()) == RETURNED_FROM_JUMP)
    {
        return;
    }

    signal(SIGVTALRM, &quantum_end_handler);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = quantum_usecs;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = quantum_usecs;

    if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
        throw std::runtime_error("Unable to set timer.");
    }
}
