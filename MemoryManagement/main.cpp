#include "Chunk.h"
#include "FixedAllocator.h"
#include <iostream>

#ifndef globalOverride
#define globalOverride
#endif // !globalOverride

void ChunkTest();
void FixedAllocatorTest();

void main() {

	FixedAllocatorTest();

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
	int* p4 = (int*)fa.Allocate();
}