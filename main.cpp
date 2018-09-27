#include "threads_manager.h"
#include <iostream>


void a()
{
    for(long i=0; i < 5000000000; i++)
    {
        if (i%10000000 == 0){
            std::cout << "In a: " << i << std::endl;
            }
    }

}

void b()
{
    for(long i=0; i < 5000000000; i++)
    {
        if (i%1000000 == 0){
            std::cout << "In b: " << i << std::endl;
            }
    }

}

int main()
{
    ThreadsManager manager(1);
    manager.spawn_thread(&a, 1);
    manager.spawn_thread(&b, 2);
    for(long i=0; i < 50000000000; i++)
    {
        if (i%100000 == 0)
        {
            std::cout << "In main: " << i << std::endl;
        }
    }
}