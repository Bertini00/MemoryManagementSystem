#include "MM_GlobalOverload.h"
#include <iostream>

#define globalOverload  false

#if globalOverload
void* operator new(size_t size)
{
    //global code here
    std::cout << "New operator overloading " << std::endl;
    void* p = malloc(size);
    return p;
}



void operator delete(void* p)
{
    std::cout << "Delete operator overloading " << std::endl;
    free(p);
}
#endif // boold
