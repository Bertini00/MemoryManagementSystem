
#include "MM_MemoryManager.h"

#include <malloc.h>

// static members need definitions
size_t MemoryManager::BOA_sizeAllocation = 1024;
size_t MemoryManager::BOA_startingObjectSize = 32;
unsigned char MemoryManager::SOA_chunkSize = 255;
size_t MemoryManager::SOA_maxObjectSize = 32;
BigObjectAllocator* MemoryManager::big_obj_alloc = nullptr;
SmallObjectAllocator* MemoryManager::small_obj_alloc = nullptr;

void MemoryManager::Init()
{
	big_obj_alloc = (BigObjectAllocator*)malloc(sizeof(BigObjectAllocator));
	new(big_obj_alloc) BigObjectAllocator(BOA_sizeAllocation, BOA_startingObjectSize);

	small_obj_alloc = (SmallObjectAllocator*)malloc(sizeof(SmallObjectAllocator));
	new(small_obj_alloc) SmallObjectAllocator(SOA_chunkSize, SOA_maxObjectSize);
}

void MemoryManager::Free()
{
	free(big_obj_alloc);
	big_obj_alloc = nullptr;

	free(small_obj_alloc);
	small_obj_alloc = nullptr;
}

void* MemoryManager::MM_Malloc(size_t size)
{
	// delegate allocation
	if (size <= SOA_maxObjectSize)
	{
		// small

		return small_obj_alloc->Allocate(size);
	}
	else
	{
		// big

		return big_obj_alloc->Allocate(size);
	}

}

void MemoryManager::MM_Free(void* pointer, size_t size)
{
	// delegate deallocation
	if (size <= SOA_maxObjectSize)
	{
		// small

		small_obj_alloc->Deallocate(pointer, size);
	}
	else
	{
		// big

		big_obj_alloc->Deallocate(pointer, size);
	}

}
