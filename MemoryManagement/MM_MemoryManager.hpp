
#pragma once

#include <cstddef>

#pragma region MACROS

#define MM_NEW(T) MemoryManager::MM_New<T>();

#define MM_NEW_A(T, length) MemoryManager::MM_New_A<T>(length);

#define MM_DELETE(T, pointer) MemoryManager::MM_Delete<T>(pointer);

#define MM_DELETE_A(T, pointer) MemoryManager::MM_Delete_A<T>(pointer);

#define MM_MALLOC(size) MemoryManager::MM_Malloc(size);

#define MM_FREE(pointer, size) MemoryManager::MM_Free(pointer, size);

#pragma endregion

#pragma region GLOBAL_VARIABLES
int max_SOA_size = 8;
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
		NewType* newPtr = (NewType*)nullptr;
		// if the size of the object is smaller or equal than the maximum size handled by the small object allocator
		if (sizeof(NewType) <= max_SOA_size) {
			// allocation
			newPtr = (NewType*)small_obj_alloc->Allocate(sizeof(NewType));

			// construction
			*newPtr = NewType();
		}
		else {
			//call Big Object Allocator here
		}
		

		return newPtr;
	}

	/*
	@brief
	Class-specific array allocation and constructor call, using small object allocator
	@param length: the length of the array to allocate
	*/
	template<class NewType>
	static NewType* MM_New_A(char length)
	{
		// allocation
		NewType* newPtr = (NewType*)small_obj_alloc->Allocate(sizeof(NewType) * length + 1);

		// write length at the beginning of the pointer
		*(char*)newPtr = length;

		// take pointer to return
		newPtr = (NewType*)((char*)newPtr + 1);

		// construction call for each object
		for (char i = 0; i < length; ++i)
		{
			newPtr[i] = NewType();
		}

		return newPtr;
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

		// if the size of the object is smaller or equal than the maximum size handled by the small object allocator
		if (sizeof(DeleteType) <= max_SOA_size) {
			// deallocation
			small_obj_alloc->Deallocate(pointer, sizeof(DeleteType));
		}
		else {
			//call Big Object Allocator here
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
	static void* MM_Malloc(std::size_t size)
	{
		// if the size of the object is smaller or equal than the maximum size handled by the small object allocator
		if (size <= max_SOA_size) {
			return small_obj_alloc->Allocate(size);
		}
		else {
			//call Big Object Allocator here
			return nullptr;
		}
		
	}

	/*
	@brief
	Deallocation, using small object allocator or big object allocator
	@param pointer: pointer to the memory to deallocate
	@param size: the size of the memory owned by the pointer
	*/
	static void MM_Free(void* pointer, std::size_t size)
	{
		// if the size of the object is smaller or equal than the maximum size handled by the small object allocator
		if (size <= max_SOA_size) {
			small_obj_alloc->Deallocate(pointer, size);
		}
		else {
			//call Big Object Allocator here
			small_obj_alloc->Deallocate(pointer, size);
		}
		
	}

private:

	static SmallObjectAllocator* small_obj_alloc;

	static unsigned char chunkSize;
	static std::size_t maxObjectSize;

};

// static member needs definition
unsigned char MemoryManager::chunkSize = 255;
std::size_t MemoryManager::maxObjectSize = max_SOA_size;
SmallObjectAllocator* MemoryManager::small_obj_alloc = new SmallObjectAllocator(MemoryManager::chunkSize, MemoryManager::maxObjectSize);
