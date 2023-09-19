
#pragma once

#include <cstddef>

#pragma region MACROS

#define MM_NEW(T) MemoryManager::MM_New<T>();

#define MM_NEW_A(T) MemoryManager::MM_New_A<T>();

#define MM_DELETE(T) MemoryManager::MM_Delete<T>();

#define MM_DELETE_A(T) MemoryManager::MM_Delete_A<T>();

#define MM_MALLOC(size) MemoryManager::MM_Malloc(size);

#define MM_FREE(pointer) MemoryManager::MM_Free(pointer);

#pragma endregion MACROS

class MemoryManager
{

public:

	/*
	Class-specific allocation and constructor call, using small object allocator
	*/
	template<class T>
	static T* MM_New()
	{

	}

	template<class T>
	static T* MM_New_A()
	{

	}

	/*
	Class-specific deallocation and destructor call, using small object allocator
	*/
	template<class T>
	static void MM_Delete(const T* pointer)
	{

	}

	template<class T>
	static void MM_Delete_A(const T* pointer)
	{

	}

	/*
	Allocation, using small object allocator
	*/
	static void* MM_Malloc(std::size_t size)
	{

	}

	/*
	Deallocation, using small object allocator
	*/
	static void MM_Free(const void* pointer)
	{

	}

private:

};
