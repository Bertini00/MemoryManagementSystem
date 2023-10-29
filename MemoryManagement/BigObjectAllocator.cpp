#include "BigObjectAllocator.h"

#include <cmath>

BigObjectAllocator::BigObjectAllocator(unsigned char chunkSize, std::size_t startingObjectSize, FitType fitType) :chunkSize_(chunkSize), startingObjectSize_(startingObjectSize), fitType_(fitType)
{
}

void* BigObjectAllocator::Allocate(std::size_t numBytes) {

	return;
}

void BigObjectAllocator::Deallocate(void* p, std::size_t size) {
	return;
}
