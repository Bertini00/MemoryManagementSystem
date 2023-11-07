#pragma once

#include "SmallObjectAllocator.h"
#include "BigObjectAllocator.h"

#pragma region MACROS

#ifdef GLOBAL_OVERLOAD

#define MM_NEW(T) new T;

#define MM_NEW_A(T, length) new T[length];

#define MM_DELETE(pointer) delete pointer;

#define MM_DELETE_A(pointer) delete[] pointer;

#else

#define MM_NEW(T) MemoryManager::MM_New<T>();

#define MM_NEW_A(T, length) MemoryManager::MM_New_A<T>(length);

#define MM_DELETE(T, pointer) MemoryManager::MM_Delete<T>(pointer);

#define MM_DELETE_A(T, pointer) MemoryManager::MM_Delete_A<T>(pointer);

#endif // !GLOBAL_OVERLOAD

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
	static void Init();

	/*
	@brief
	Free of all the resources owned by the memory manager - needed at the end of the usage
	*/
	static void Free();

#ifndef GLOBAL_OVERLOAD

	/*
	@brief
	Class-specific allocation and constructor call, using small object allocator or big object allocator
	*/
	template<class NewType>
	static NewType* MM_New()
	{
		// delegate allocation
		if (sizeof(NewType) <= SOA_maxObjectSize)
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
			numBytes += 2; // size of unsigned char for metadata size info + size of another unsigned char for actual length
		}
		else
		{
			numBytes += 1 + sizeof(size_t); // size of unsigned char for metadata size info + size of size_t for actual length
		}

		// delegate allocation
		if (numBytes <= SOA_maxObjectSize)
		{
			// small

			// allocation
			unsigned char* ptr = (unsigned char*)small_obj_alloc->Allocate(numBytes);

			// write metadata at the beginning of the pointer
			*ptr = (unsigned char)length; // taken as unsigned char because for longer arrays -> the allocation will be taken from the BOA
			*(ptr + 1) = 1; // size of unsigned char (metadata size)

			// take pointer to return
			NewType* newPtr = (NewType*)(ptr + 2);

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
			unsigned char* ptr = (unsigned char*)big_obj_alloc->Allocate(numBytes);

			// write metadata at the beginning of the pointer
			*(size_t*)ptr = length;
			*(ptr + 1) = sizeof(size_t); // size of size_t (metadata size)

			// take pointer to return
			NewType* newPtr = (NewType*)(ptr + sizeof(size_t) + 1);

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

		// delegate deallocation
		if (sizeof(DeleteType) <= SOA_maxObjectSize)
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
		// take array length from metadata
		unsigned char* ptr = (unsigned char*)pointer;
		unsigned char metaSize = *(ptr - 1); // metadata size
		ptr = ptr - 1 - metaSize; // move ptr to actual array length data -> pointer to actual allocation

		size_t length;
		if (metaSize == 1)
		{
			// length as unsigned char

			length = *(ptr);
		}
		else
		{
			// length as size_t

			length = *((size_t*)ptr);
		}

		// actual allocation size
		size_t numBytes = sizeof(DeleteType) * length + 1 + metaSize;

		// destruction
		for (size_t i = 0; i < length; ++i)
		{
			pointer[i].~DeleteType();
		}

		// delegate deallocation
		if (numBytes <= SOA_maxObjectSize)
		{
			// small

			small_obj_alloc->Deallocate(ptr, numBytes);
		}
		else
		{
			// big

			big_obj_alloc->Deallocate(ptr, numBytes);
		}

		// make pointer null
		pointer = nullptr;
	}

#endif // GLOBAL_OVERLOAD

	/*
	@brief
	Allocation, using small object allocator or big object allocator
	@param size: the size (in bytes) of memory to allocate
	*/
	static void* MM_Malloc(size_t size);

	/*
	@brief
	Deallocation, using small object allocator or big object allocator
	@param pointer: pointer to the memory to deallocate
	@param size: the size of the memory owned by the pointer
	*/
	static void MM_Free(void* pointer, size_t size);

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
