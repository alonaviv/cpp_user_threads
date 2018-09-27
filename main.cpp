#include "threads_manager.h"
#include <iostream>

ThreadsManager manager(100000);

void a()
{
    for(long i=1; i < 50000000000; i++)
    {
        if (i%10000000 == 0)
        {
            std::cout << "In a: " << i << std::endl;
        }
        if (i%100000000 == 0){
            std::cout << "Termnating thread 2" << std::endl;
            manager.terminate_thread(2);}

    }
}

void b()
{
    for(long i=1; i < 50000000000; i++)
    {
        if (i%10000000 == 0){
            std::cout << "In b: " << i << std::endl;
            }


    }
}

int main()
{
    manager.spawn_thread(&a, 1);
    manager.spawn_thread(&b, 2);

    for(long i=1; i < 500000000; i++)
    {
        if (i%10000000 == 0)
        {
            std::cout << "In main: " << i << std::endl;
        }

        if (i%200000000 == 0){
            std::cout << "Termination thread 1" << std::endl;
            manager.terminate_thread(1);}
    }
}