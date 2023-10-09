#pragma once

#include <vector>

#include "FixedAllocator.h"

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

private:
	unsigned char chunkSize_;
	size_t startingObjectSize_;
	FitType fitType_;

	std::vector<FixedAllocator> pool_;
	FixedAllocator* pLastAlloc_;
	FixedAllocator* pLastDealloc_;
};
