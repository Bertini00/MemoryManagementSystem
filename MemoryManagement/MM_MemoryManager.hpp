
#pragma once

#include <cstddef>

#pragma region MACROS

#define MM_NEW(T) MemoryManager::MM_New<T>();

#define MM_NEW_A(T) MemoryManager::MM_New_A<T>();

#define MM_DELETE(T, pointer) MemoryManager::MM_Delete<T>(pointer);

#define MM_DELETE_A(T) MemoryManager::MM_Delete_A<T>();

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
	Initialization of the memory manager - needed before any other function call
	@param chunkSize: the size of the chunks to use
	@param maxObjectSize: the maximum size handled by the small object allocator
	*/
	static void Init(unsigned char chunkSize, std::size_t maxObjectSize)
	{
		if (small_obj_alloc != nullptr)
			Free();

		small_obj_alloc = new SmallObjectAllocator(chunkSize, maxObjectSize);
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
	Class-specific allocation and constructor call, using small object allocator
	*/
	template<class NewType>
	static NewType* MM_New()
	{
		// allocation
		NewType* newPtr = (NewType*)small_obj_alloc->Allocate(sizeof(NewType));
		
		// construction
		*newPtr = NewType();

		return newPtr;
	}

	template<class T>
	static T* MM_New_A()
	{
		return nullptr;
	}

	/*
	@brief
	Class-specific deallocation and destructor call, using small object allocator
	@param pointer: pointer to object to destroy and deallocate
	*/
	template<class DeleteType>
	static void MM_Delete(DeleteType* pointer)
	{
		// destruction
		pointer->~DeleteType();

		// deallocation
		small_obj_alloc->Deallocate(pointer, sizeof(DeleteType));
	}

	template<class T>
	static void MM_Delete_A(T* pointer)
	{

	}

	/*
	@brief
	Allocation, using small object allocator
	@param size: the size (in bytes) of memory to allocate
	*/
	static void* MM_Malloc(std::size_t size)
	{
		return small_obj_alloc->Allocate(size);
	}

	/*
	@brief
	Deallocation, using small object allocator
	@param pointer: pointer to the memory to deallocate
	@param size: the size of the memory owned by the pointer
	*/
	static void MM_Free(void* pointer, std::size_t size)
	{
		small_obj_alloc->Deallocate(pointer, size);
	}

private:

	static SmallObjectAllocator* small_obj_alloc;

};

// static member needs definition
SmallObjectAllocator* MemoryManager::small_obj_alloc = nullptr;
