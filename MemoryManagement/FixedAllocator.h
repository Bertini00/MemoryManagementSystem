#pragma once
#include <vector>
#include "Chunk.h"
#include "Mallocator.h"

class FixedAllocator
{

public:
	FixedAllocator(size_t blockSize, unsigned char numBlocks);

	void* Allocate();
	void Deallocate(void* p);

	size_t GetBlockSize();

	/*
	* Frees all the resources of the FixedAllocator
	*/
	void Free();

private:
	size_t blockSize_;
	unsigned char numBlocks_;

	typedef std::vector<Chunk, Mallocator<Chunk>> Chunks;

	Chunks chunks_;
	Chunk* allocChunk_;
	Chunk* deallocChunk_;
};