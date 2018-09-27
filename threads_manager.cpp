#include "threads_manager.h"
#include "thread.h"

#include <iostream>
#include <csignal>
#include <stdexcept>
#include <iostream>

std::shared_ptr<Thread> ThreadsManager::running_thread = std::make_shared<Thread>(0);
std::queue<std::shared_ptr<Thread>> ThreadsManager::ready_queue;
std::vector<std::shared_ptr<Thread>> ThreadsManager::all_threads;
struct sigaction ThreadsManager::sa;
Timer ThreadsManager::timer;


void ThreadsManager::quantum_end_handler(int sig_number)
{
    std::cout << "DING DONG!!" << std::endl;
    sigset_t unblocking_signal_set;


    if(setjmp(running_thread -> get_buffer()) == RETURNED_FROM_JUMP)
    {
        return;
    }
    std::cout << "Preempting thread " << running_thread -> get_id() << std::endl;

    if(running_thread -> get_state() == TERMINATED)
    {
        running_thread.reset();
    } else
    {
        running_thread -> set_state(READY);
        ready_queue.push(std::move(running_thread));
    }

    do
    {
        running_thread = std::move(ready_queue.front());
        if(running_thread -> get_state() == TERMINATED)
            std::cout << "Removing and deleting thread " << running_thread -> get_id() << std::endl;

        ready_queue.pop();
    } while(running_thread -> get_state() == TERMINATED);

    std::cout << "Running thread " << running_thread -> get_id() << std::endl;
    running_thread->set_state(RUNNING);

    timer.start_timer();
    sigaddset(&unblocking_signal_set, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &unblocking_signal_set, nullptr);
    sigaction(SIGVTALRM, &sa, nullptr);

    longjmp(running_thread -> get_buffer(), RETURNED_FROM_JUMP);

}


ThreadsManager::ThreadsManager(unsigned int quantum_usecs)
{
    all_threads.push_back(running_thread);
    if(setjmp(running_thread -> get_buffer()) == RETURNED_FROM_JUMP)
    {
        return;
    }

    sa.sa_handler = &quantum_end_handler;
    sigaction(SIGVTALRM, &sa, nullptr);
    timer.set_timer(quantum_usecs);
    timer.start_timer();
}


void ThreadsManager::spawn_thread(void (*f)(), unsigned int thread_id)
{
    std::cout << "Spawning thread " << thread_id << " into ready list" << std::endl;
    all_threads.insert(all_threads.begin() + thread_id, std::make_shared<Thread>(thread_id, f));
    ready_queue.push(all_threads[thread_id]);
}

void ThreadsManager::terminate_thread(unsigned int thread_id)
{
    if(all_threads[thread_id])
    {
        all_threads[thread_id]->update_state(TERMINATED);
        all_threads[thread_id].reset();
    }
}

void Timer::set_timer(unsigned int timer_usecs)
{
    this -> timer_usecs = timer_usecs;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = timer_usecs;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = timer_usecs;
}

void Timer::start_timer()
{
    if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
        throw std::runtime_error("Unable to set timer.");
    }

}