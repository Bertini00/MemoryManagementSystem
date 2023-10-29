#pragma once

#include <vector>

#include "BOA_Block.h"

enum FitType
{
	BEST_FIT,
	WORST_FIT,
	FIRST_FIT
};

class BigObjectAllocator
{
public:
	BigObjectAllocator(unsigned char chunkSize, std::size_t startingObjectSize, FitType fitType = BEST_FIT);

	void* Allocate(std::size_t numBytes);
	void Deallocate(void* p, std::size_t size);

	void joinNearBlocks(BOA_Block* block);

private:
	unsigned char chunkSize_;
	size_t startingObjectSize_;
	FitType fitType_;

	
	std::vector<BOA_Block> pool_;
	BOA_Block* pLastAlloc_;
	BOA_Block* pLastDealloc_;
};
