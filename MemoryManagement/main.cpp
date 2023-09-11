#include "Chunk.h"
#include <iostream>

void main() {

	std::cout << sizeof(int) << std::endl;

	Chunk* chunk = new Chunk();

	chunk->Init(sizeof(int), 20);


	int* p1 = (int*)chunk->Allocate(sizeof(int));
	*p1 = 189;
	
	int* p2 = (int*)chunk->Allocate(sizeof(int));
	*p2 = 231;

	chunk->Deallocate(p1, sizeof(int));

	int* p3 = (int*)chunk->Allocate(sizeof(int));
	*p3 = 639;

	int* p4 = (int*)chunk->Allocate(sizeof(int));
	*p4 = 9287;

	chunk->Deallocate(p3, sizeof(int));
	chunk->Deallocate(p2, sizeof(int));
	chunk->Deallocate(p4, sizeof(int));

	delete chunk;
}