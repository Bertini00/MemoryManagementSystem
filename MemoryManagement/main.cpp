#include "Chunk.h"
#include "FixedAllocator.h"
#include "SmallObjectAllocator.h"

#include "MM_MemoryManager.hpp"

#include <iostream>

#ifndef globalOverride
#define globalOverride
#endif // !globalOverride

void ChunkTest();
void FixedAllocatorTest();
void SmallObjectAllocatorTest();
void MemoryManagerTest();

struct TestStruct
{
	int value;

	~TestStruct()
	{
		value = 0;
	}
};

void main() {


	//SmallObjectAllocatorTest();
	//FixedAllocatorTest();

	//FixedAllocatorTest();

	MemoryManagerTest();
}



void MemoryManagerTest()
{
	MemoryManager::Init(255, 32);

	int* p1 = MM_NEW(int);
	MM_DELETE(int, p1);

	float* p2 = (float*)MM_MALLOC(sizeof(float));
	MM_FREE(p2, sizeof(float));

	int* a1 = MM_NEW_A(int, 3);
	a1[0] = 1;
	MM_DELETE_A(int, a1);

	TestStruct* st = MM_NEW_A(TestStruct, 2);
	st[0].value = 1;
	MM_DELETE_A(TestStruct, st);

	MemoryManager::Free();
}

void ChunkTest() {
	std::cout << sizeof(int) << std::endl;

	Chunk* chunk = new Chunk();
	Chunk* chunk2 = new Chunk();

	chunk->Init(sizeof(int), 20);
	chunk2->Init(sizeof(int), 20);


	int* p1 = (int*)chunk->Allocate(sizeof(int));
	*p1 = 189;

	int* p2 = (int*)chunk2->Allocate(sizeof(int));
	int* p3 = (int*)chunk->Allocate(sizeof(int));
	int* p4 = (int*)chunk->Allocate(sizeof(int));

	chunk->Deallocate(p2, sizeof(p1));
	chunk2->Deallocate(p4, sizeof(p4));

	delete chunk;
	delete chunk2;
}

void FixedAllocatorTest() {
	FixedAllocator fa = FixedAllocator(sizeof(int), 2);

	

	int* p1 = (int*)fa.Allocate();
	int* p2 = (int*)fa.Allocate();
	int* p3 = (int*)fa.Allocate();

	fa.Deallocate(p1);
	fa.Deallocate(p3);
	int* p4 = (int*)fa.Allocate();
}

void SmallObjectAllocatorTest() {
	SmallObjectAllocator sa = SmallObjectAllocator(2, 32);

	int* p1 = (int*)sa.Allocate(sizeof(int));
	*p1 = 12341;
	int* p2 = (int*)sa.Allocate(sizeof(int));
	*p2 = 81721;

	int* p3 = (int*)sa.Allocate(sizeof(int));
	char* p4 = (char*)sa.Allocate(sizeof(char));
	char* p5 = (char*)sa.Allocate(sizeof(char));
	char* p6 = (char*)sa.Allocate(sizeof(char));

	sa.Deallocate(p1, sizeof(int));
	sa.Deallocate(p2, sizeof(int));
	sa.Deallocate(p5, sizeof(char));
	sa.Deallocate(p6, sizeof(char));
}