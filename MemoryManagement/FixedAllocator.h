#pragma once
#include <vector>
#include "Chunk.h"

class FixedAllocator 
{

public:
	void* Allocate();

private: 
	size_t blockSize_; 
	unsigned char numBlocks_; 

	typedef std::vector<Chunk> Chunks; 

	Chunks chunks_; 
	Chunk* allocChunk_; 
	Chunk* deallocChunk_; 
};