#include <iostream>

#include "thread.h"


//Thread::Thread(unsigned id, void (*f)())
//{
//
//}


Thread::~Thread()
{
    delete thread_buffer;
    delete private_stack;
}
