#pragma once
#include <vector>
#include "Chunk.h"

class FixedAllocator
{

public:
	FixedAllocator(size_t blockSize, unsigned char numBlocks);

	void* Allocate();
	void Deallocate(void* p);

private:
	size_t blockSize_;
	unsigned char numBlocks_;

	typedef std::vector<Chunk> Chunks;

	Chunks chunks_;
	Chunk* allocChunk_;
	Chunk* deallocChunk_;
};