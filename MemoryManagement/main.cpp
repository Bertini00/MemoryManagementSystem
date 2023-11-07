
#include "MM_GlobalOverload.h" // include this header to enable global operator new and operator delete overload -> it HAS to be included before MM_MemoryManager.h
#include "MM_MemoryManager.h"

#include "RedBlackTree/RBTree.h"

#include <iostream>
#include <chrono>

void ChunkTest();
void FixedAllocatorTest();
void SmallObjectAllocatorTest();
void MemoryManagerTest();
void RBTreeTest();

struct TestStruct
{
	int value;

	~TestStruct()
	{
		value = 0;
	}
};
void BigObjectAllocatorTest();

void main() {


	//SmallObjectAllocatorTest();
	//FixedAllocatorTest();

	//FixedAllocatorTest();

	MemoryManagerTest();

	//RBTreeTest();
	//BigObjectAllocatorTest();
}


void RBTreeTest()
{
	RBTree tree = RBTree();

	std::chrono::high_resolution_clock::time_point begin;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::milliseconds elapsed;


#pragma region 1

	std::cout << "Inserts:" << std::endl;

	begin = std::chrono::high_resolution_clock::now();

	int x = 1;
	void* value = nullptr;
	tree.Insert(8, value);
	tree.Insert(20, value);
	tree.Insert(6, value);
	tree.Insert(0, value);
	tree.Insert(10, value);
	tree.Insert(9, &x);
	tree.Insert(7, value);
	tree.Insert(21, value);
	tree.Insert(22, value);

	end = std::chrono::high_resolution_clock::now();

	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

	tree.Print();

	std::cout << "Deletes:" << std::endl;

	// delete tests
	tree.Delete(10);
	tree.Delete(20);
	tree.Delete(8);

	tree.Print();

	// print
	std::cout << "9 insert elapsed time: " << elapsed.count() << " ms" << std::endl;

	RBNode* around = tree.LookUpAtLeast(18);
	if (around != nullptr)
		std::cout << around->key << std::endl;
	around = tree.LookUpAtLeast(23);
	if (around != nullptr)
		std::cout << around->key << std::endl;

	// test of delete on same key with different values
	std::cout << "Same key delete tests" << std::endl;
	int y = 5;
	tree.Insert(9, &y);
	tree.Print();
	tree.Delete(9, &y);
	tree.Print();

#pragma endregion 1
}

void MemoryManagerTest()
{
	MemoryManager::Init();

	int* x = new int;

#ifdef GLOBAL_OVERLOAD

	int* p1 = MM_NEW(int);
	MM_DELETE(p1);

	float* p2 = (float*)MM_MALLOC(sizeof(float));
	MM_FREE(p2, sizeof(float));

	int* a1 = MM_NEW_A(int, 3);
	a1[0] = 1;
	MM_DELETE_A(a1);

	TestStruct* st = MM_NEW_A(TestStruct, 2);
	st[0].value = 1;
	MM_DELETE_A(st);

#else

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

#endif // GLOBAL_OVERLOAD

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

void BigObjectAllocatorTest() {
	BigObjectAllocator boa = BigObjectAllocator(1024, 32);

	int* i = (int*)boa.Allocate(sizeof(int));
	*i = 255;
	int* j = (int*)boa.Allocate(sizeof(int));
	int* k = (int*)boa.Allocate(sizeof(int));
	int* w = (int*)boa.Allocate(sizeof(int));

	boa.Deallocate(i, sizeof(i));
	boa.Deallocate(k, sizeof(k));

	i = (int*)boa.Allocate(sizeof(int));
}