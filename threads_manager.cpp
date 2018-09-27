#include "threads_manager.h"
#include "thread.h"

#include <iostream>
#include <stdexcept>
#include <iostream>

std::shared_ptr<Thread> ThreadsManager::running_thread = std::make_shared<Thread>(0);
std::queue<std::shared_ptr<Thread>> ThreadsManager::ready_queue;
std::vector<std::shared_ptr<Thread>> ThreadsManager::all_threads;


void ThreadsManager::quantum_end_handler(int sig_number)
{
    std::cout << "DING DONG!!" << std::endl;

    if(setjmp(running_thread -> get_buffer()) == RETURNED_FROM_JUMP)
    {
        return;
    }
    std::cout << "Preempting thread " << running_thread -> get_id() << std::endl;

    running_thread -> set_state(READY);
    ready_queue.push(std::move(running_thread));

    running_thread = std::move(ready_queue.front());
    running_thread -> set_state(RUNNING);
    ready_queue.pop();
    std::cout << "Changing to thread " << running_thread -> get_id() << std::endl;

    //signal(SIGVTALRM, &quantum_end_handler);

    longjmp(running_thread -> get_buffer(), RETURNED_FROM_JUMP);
}

ThreadsManager::ThreadsManager(unsigned int quantum_usecs): quantum_usecs(quantum_usecs)
{
    all_threads.push_back(running_thread);
    if(setjmp(running_thread -> get_buffer()) == RETURNED_FROM_JUMP)
    {
        return;
    }

    sa.sa_handler = &quantum_end_handler;
    if (sigaction(SIGVTALRM, &sa, NULL) < 0) {
        throw std::runtime_error("sigaction error.");
    }

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = quantum_usecs;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = quantum_usecs;

    if (setitimer (ITIMER_VIRTUAL, &timer, NULL)) {
        throw std::runtime_error("Unable to set timer.");
    }
}

void ThreadsManager::spawn_thread(void (*f)(), unsigned int thread_id)
{
    std::cout << "Spawning thread " << thread_id << " into ready list" << std::endl;
    all_threads.insert(all_threads.begin() + thread_id, std::make_shared<Thread>(thread_id, f));
    ready_queue.push(all_threads[thread_id]);
}