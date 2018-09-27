#include <iostream>
#include <csignal>

#include "thread.h"


/* code for 64 bit Intel arch */

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
                 "rol    $0x11,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}


Thread::Thread(unsigned id): id(id), state(RUNNING) {setjmp(thread_buffer);}

Thread::Thread(unsigned id, void (*f)()): id(id), state(READY), private_stack(new char[PRIVATE_STACK_SIZE])
{
    address_t sp, pc;
    sp = (address_t)private_stack.get() + PRIVATE_STACK_SIZE - sizeof(address_t);
    pc = (address_t)f;

    setjmp(thread_buffer);
    thread_buffer ->__jmpbuf[JB_SP] = translate_address(sp);
    thread_buffer ->__jmpbuf[JB_PC] = translate_address(pc);

    sigemptyset(&(thread_buffer -> __saved_mask));
}