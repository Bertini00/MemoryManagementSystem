#pragma once

/*
* This is the global new operator overload.
* It is called when the user calls new.
* @param size_t size - The size of the object to be allocated.
*/
void* operator new(size_t size);

/*
* This is the global delete operator overload.
* It is called when the user calls delete.
* @param void* p - The pointer to the object to be deleted.
*/
void operator delete(void*);
