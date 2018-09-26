#include <csetjmp>
#include <bits/unique_ptr.h>

#ifndef CPP_STUFF_THREADS_H
#define CPP_STUFF_THREADS_H

const unsigned int PRIVATE_STACK_SIZE = 4096;


class Thread
{
private:
    const unsigned id;
    jmp_buf thread_buffer;
    const std::unique_ptr<char[]> private_stack;

public:
    //Constructor for thread #0 (main program initializing the library)
    explicit Thread(unsigned id): id(id), private_stack(nullptr) {};
    //Thread(unsigned int id, void (*f)()); // Constructor for spawning new threads
    unsigned get_id() const { return id;}
    __jmp_buf_tag* get_buffer() { return thread_buffer;}
};

#endif //CPP_STUFF_THREADS_H
