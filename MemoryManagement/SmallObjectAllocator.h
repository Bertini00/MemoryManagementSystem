#pragma once
#include <vector>

#include "FixedAllocator.h"


class SmallObjectAllocator
{
public:

	SmallObjectAllocator(std::size_t chunkSize, std::size_t maxObjectSize); 
	void* Allocate(std::size_t numBytes); 
	void Deallocate(void* p, std::size_t size);

private:
	size_t chunkSize_;
	size_t maxObjectSize_;


	std::vector<FixedAllocator> pool_;
	FixedAllocator* pLastAlloc_; 
	FixedAllocator* pLastDealloc_;
};

