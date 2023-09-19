#include "MM_GlobalOverload.h"
#include <iostream>

/*
* This is the global new operator overload.
* It is called when the user calls new.
* @param size_t size - The size of the object to be allocated.
*/
void* operator new(size_t size)
{
    //global code here
    std::cout << "New operator overloading " << std::endl;
    void* p = malloc(size);
    return p;
}


/*
* This is the global delete operator overload.
* It is called when the user calls delete.
* @param void* p - The pointer to the object to be deleted.
*/
void operator delete(void* p)
{
    std::cout << "Delete operator overloading " << std::endl;
    free(p);
}
