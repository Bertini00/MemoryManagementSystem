#include "BigObjectAllocator.h"

#include <cmath>

BigObjectAllocator::BigObjectAllocator(unsigned char chunkSize, std::size_t startingObjectSize, FitType fitType) :chunkSize_(chunkSize), startingObjectSize_(startingObjectSize), fitType_(fitType)
{
}

void* BigObjectAllocator::Allocate(std::size_t numBytes) {

	void* result = nullptr;
	int valueToSearch;

	// Allocate the correct chunk based on the fitType
	switch (fitType_)
	{

	case BEST_FIT:
		// Search which fixed allocator has the correct size

		// Calculate the correct size of the fixed allocator to search
		valueToSearch = std::exp2(std::ceil(std::log2(numBytes)));

		for (int i = 0; i < pool_.size(); i++)
		{
			// If it's the correct size, then allocate and assign the return value to result
			if (pool_[i].GetBlockSize() == valueToSearch)
			{
				result = pool_[i].Allocate();
				break;
			}
		}
		break;
	case WORST_FIT:

		// Save the biggest Fixed allocator with its size
		// (Use a pointer to save space)
		FixedAllocator* currentFA = nullptr;
		int currentBlockSize = 0;

		for (int i = 0; i < pool_.size(); i++)
		{
			// Check if the current block size is bigger than the one saved
			if (pool_[i].GetBlockSize() > currentBlockSize)
			{
				// Saved = current
				currentBlockSize = pool_[i].GetBlockSize();
				currentFA = &pool_[i];
			}
		}

		// Check if there is a chunk big enough to allocate
		if (currentFA != nullptr)
		{
			result = currentFA->Allocate();
		}
		break;
	case FIRST_FIT:

		for (int i = 0; i < pool_.size(); i++)
		{
			// Get the first allocator available and allocate
			if (pool_[i].GetBlockSize() > numBytes)
			{
				result = pool_[i].Allocate();
				break;
			}
		}
		break;
	default:
		
		break;
	}

	// Check if there was a allocator big enough to store the bytes passed
	if (result != nullptr)
	{
		return result;
	}
	else
	{
		// Create a fixed allocator big enough of return the nullpointer because there is no space available?
	}


	return result;
}

void BigObjectAllocator::Deallocate(void* p, std::size_t size) {
	return;
}