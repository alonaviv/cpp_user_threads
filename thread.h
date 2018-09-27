#include <csetjmp>
#include <bits/unique_ptr.h>

#ifndef CPP_STUFF_THREADS_H
#define CPP_STUFF_THREADS_H

const unsigned int PRIVATE_STACK_SIZE = 4096;

enum StateType {RUNNING, TERMINATED, READY, BLOCKED, SLEEPING};

class Thread
{
private:
    const unsigned int id;
    StateType state;
    jmp_buf thread_buffer;
    const std::unique_ptr<char[]> private_stack;

public:
    //Constructor for thread #0 (main program initializing the library)
    explicit Thread(unsigned id);
    // Constructor for spawning new threads
    Thread(unsigned int id, void (*f)());
    unsigned get_id() const { return id;}
    __jmp_buf_tag* get_buffer() { return thread_buffer;}
    void set_state(StateType state) {this -> state = state;}
    StateType get_state() {return state;}
    void update_state(StateType state) {this -> state = state;}

};


#endif //CPP_STUFF_THREADS_H
