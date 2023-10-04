#pragma once
#include <vector>

#include "FixedAllocator.h"


/*
	Small Object Allocator handles up to 16 bytes (to be decided yet) then it delegates the new to the Big Object Allocator
	Each allocation in the SOA creates the corresponding fixed allocator with that size (array of 3 int creates a fixed of 12 bytes)
*/

class SmallObjectAllocator
{
public:

	SmallObjectAllocator(unsigned char chunkSize, std::size_t maxObjectSize); 
	void* Allocate(std::size_t numBytes); 
	void Deallocate(void* p, std::size_t size);

private:
	unsigned char chunkSize_;
	size_t maxObjectSize_;


	std::vector<FixedAllocator> pool_;
	FixedAllocator* pLastAlloc_; 
	FixedAllocator* pLastDealloc_;
};

