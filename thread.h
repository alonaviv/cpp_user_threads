#include <csetjmp>

#ifndef CPP_STUFF_THREADS_H
#define CPP_STUFF_THREADS_H

const unsigned int PRIVATE_STACK_SIZE = 4096;


class Thread
{
private:
    sigjmp_buf * const thread_buffer;
    const unsigned id;
    char * const private_stack;

public:
    //Constructor for thread #0 (main program initializing the library)
    explicit Thread(unsigned id): id(id), private_stack(nullptr), thread_buffer(new sigjmp_buf) {};
    //Thread(unsigned int id, void (*f)()); // Constructor for spawning new threads
    Thread(Thread const&) = delete;
    void operator=(Thread const&) = delete;
    Thread(Thread&& other) noexcept: thread_buffer(other.thread_buffer), id(other.id),
                                     private_stack(other.private_stack) {};
    ~Thread();
    unsigned get_id(){ return id;}
    sigjmp_buf * const get_buffer() { return thread_buffer;}
};

#endif //CPP_STUFF_THREADS_H
