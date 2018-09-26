#include "threads_manager.h"
#include <iostream>

int main()
{
    ThreadsManager manager(1);
    for(int i=0; i < 5000000; i++)
    {
        if (i%1000 == 0)
            std::cout << i << std::endl;
    }

}