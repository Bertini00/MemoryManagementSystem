
#pragma once

#include <malloc.h>

#pragma region MACROS

#define MM_NEW(T) MemoryManager::MM_New<T>();

#define MM_NEW_A(T, length) MemoryManager::MM_New_A<T>(length);

#define MM_DELETE(T, pointer) MemoryManager::MM_Delete<T>(pointer);

#define MM_DELETE_A(T, pointer) MemoryManager::MM_Delete_A<T>(pointer);

#define MM_MALLOC(size) MemoryManager::MM_Malloc(size);

#define MM_FREE(pointer, size) MemoryManager::MM_Free(pointer, size);

#pragma endregion

static class MemoryManager
{

public:

	/*
	Remove constructor to have a single copy of the object
	*/
	MemoryManager() = delete;

	/*
	Remove destructor to handle destruction separately
	*/
	~MemoryManager() = delete;

	/*
	@brief
	Init all the resources needed by the memory manager - needed at the start of the usage
	*/
	static void Init()
	{
		big_obj_alloc = (BigObjectAllocator*)malloc(sizeof(BigObjectAllocator));
		new(big_obj_alloc) BigObjectAllocator(MemoryManager::BOA_sizeAllocation, MemoryManager::BOA_startingObjectSize);

		small_obj_alloc = (SmallObjectAllocator*)malloc(sizeof(SmallObjectAllocator));
		new(small_obj_alloc) SmallObjectAllocator(MemoryManager::SOA_chunkSize, MemoryManager::SOA_maxObjectSize);
	}

	/*
	@brief
	Free of all the resources owned by the memory manager - needed at the end of the usage
	*/
	static void Free()
	{
		delete small_obj_alloc;
		small_obj_alloc = nullptr;
	}

	/*
	@brief
	Class-specific allocation and constructor call, using small object allocator or big object allocator
	*/
	template<class NewType>
	static NewType* MM_New()
	{
		if (sizeof(NewType) <= MemoryManager::SOA_maxObjectSize)
		{
			// small
			
			// allocation
			NewType* newPtr = (NewType*)small_obj_alloc->Allocate(sizeof(NewType));

			// construction
			new(newPtr) NewType();

			return newPtr;
		}
		else
		{
			// big

			// allocation
			NewType* newPtr = (NewType*)big_obj_alloc->Allocate(sizeof(NewType));

			// construction
			new(newPtr) NewType();

			return newPtr;
		}
	}

	/*
	@brief
	Class-specific array allocation and constructor call, using small object allocator
	@param length: the length of the array to allocate
	*/
	template<class NewType>
	static NewType* MM_New_A(size_t length)
	{
		size_t numBytes = sizeof(NewType) * length;
		if (length <= 255)
		{
			numBytes += 1; // size of unsigned char
		}
		else
		{
			numBytes += sizeof(size_t);
		}

		if (numBytes <= MemoryManager::SOA_maxObjectSize)
		{
			// small

			// allocation
			NewType* newPtr = (NewType*)small_obj_alloc->Allocate(numBytes);

			// write length at the beginning of the pointer
			*(unsigned char*)newPtr = (unsigned char)length; // taken as unsigned char because for longer arrays -> the allocation will be taken from the BOA

			// take pointer to return
			newPtr = (NewType*)((unsigned char*)newPtr + 1);

			// construction call for each object
			unsigned char _length = (unsigned char)length;
			for (unsigned char i = 0; i < _length; ++i)
			{
				newPtr[i] = NewType();
			}

			return newPtr;
		}
		else
		{
			// big

			// allocation
			NewType* newPtr = (NewType*)big_obj_alloc->Allocate(numBytes);

			// write length at the beginning of the pointer
			*(size_t*)newPtr = length;

			// take pointer to return
			newPtr = (NewType*)((size_t*)newPtr + 1);

			// construction call for each object
			for (size_t i = 0; i < length; ++i)
			{
				newPtr[i] = NewType();
			}

			return newPtr;
		}
	}

	/*
	@brief
	Class-specific deallocation and destructor call, using small object allocator or big object allocator
	@param pointer: pointer to object to destroy and deallocate
	*/
	template<class DeleteType>
	static void MM_Delete(DeleteType*& pointer)
	{
		// destruction
		pointer->~DeleteType();

		if (sizeof(DeleteType) <= MemoryManager::SOA_maxObjectSize)
		{
			// small
			
			// deallocation
			small_obj_alloc->Deallocate(pointer, sizeof(DeleteType));
		}
		else
		{
			// big

			big_obj_alloc->Deallocate(pointer, sizeof(DeleteType));
		}

		// make pointer null
		pointer = nullptr;
	}

	/*
	@brief
	Class-specific array deallocation and destructor call, using small object allocator
	@param pointer: the array pointer to deallocate
	*/
	template<class DeleteType>
	static void MM_Delete_A(DeleteType*& pointer)
	{
		// PROBLEMA: se allochiamo array di small object come un singolo small object -> se la size massima è ad esempio 8 non si potranno avere array più lunghi di 8 elementi (nel caso di oggetti da 1 byte, ma è solo un esempio)
		// POSSIBILE SOLUZIONE: allocare array di small object come tante allocazioni separate, scrivendo all'inizio la lunghezza dell'array -> la dimensione dell'intero destinato a conservare la lunghezza potrebbe essere pari alla size del singolo elemento -> in questo modo si può sempre sapere in fase di deallocazione di quanto tornare indietro per ottenere la lunghezza dell'array


		//size_t numBytes = sizeof(DeleteType) * length;

		// take length
		char length = *((char*)pointer - 1);

		// destruction
		for (char i = 0; i < length; ++i)
		{
			pointer[i].~DeleteType();
		}

		// take real pointer to allocation
		pointer = (DeleteType*)((char*)pointer - 1);

		// deallocation
		small_obj_alloc->Deallocate(pointer, sizeof(DeleteType) * length + 1);

		// make pointer null
		pointer = nullptr;
	}

	/*
	@brief
	Allocation, using small object allocator or big object allocator
	@param size: the size (in bytes) of memory to allocate
	*/
	static void* MM_Malloc(size_t size)
	{
		if (size <= MemoryManager::SOA_maxObjectSize)
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

	/*
	@brief
	Deallocation, using small object allocator or big object allocator
	@param pointer: pointer to the memory to deallocate
	@param size: the size of the memory owned by the pointer
	*/
	static void MM_Free(void* pointer, size_t size)
	{
		if (size <= MemoryManager::SOA_maxObjectSize)
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

private:

	static BigObjectAllocator* big_obj_alloc;
	static SmallObjectAllocator* small_obj_alloc;

	// big
	static size_t BOA_sizeAllocation;
	static size_t BOA_startingObjectSize;

	// small
	static unsigned char SOA_chunkSize;
	static size_t SOA_maxObjectSize;

};

// static member needs definition
size_t MemoryManager::BOA_sizeAllocation = 1024;
size_t MemoryManager::BOA_startingObjectSize = 32;
unsigned char MemoryManager::SOA_chunkSize = 255;
size_t MemoryManager::SOA_maxObjectSize = 8;
BigObjectAllocator* MemoryManager::big_obj_alloc = nullptr;
SmallObjectAllocator* MemoryManager::small_obj_alloc = nullptr;
