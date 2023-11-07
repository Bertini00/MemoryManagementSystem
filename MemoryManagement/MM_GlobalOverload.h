#pragma once

#define GLOBAL_OVERLOAD

#ifdef GLOBAL_OVERLOAD

#include "MM_MemoryManager.h"

/*
* This is the global new operator overload.
* It is called when the user calls new.
* @param size_t size - The size of the object to be allocated.
*/
void* operator new(size_t size)
{
	return MemoryManager::MM_Malloc(size);
}

/*
* This is the global delete operator overload.
* It is called when the user calls delete.
* @param void* pointer - The pointer to the object to be deleted.
* @param size_t size - The size to deallocate.
*/
void operator delete(void* pointer, size_t size)
{
	MemoryManager::MM_Free(pointer, size);
}

#endif // GLOBAL_OVERLOAD
