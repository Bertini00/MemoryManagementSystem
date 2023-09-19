#include "FixedAllocator.h"
#include <assert.h>
#include <iostream>

FixedAllocator::FixedAllocator(size_t blockSize, unsigned char numBlocks) :blockSize_(blockSize), numBlocks_(numBlocks) {
}

void* FixedAllocator::Allocate() {
	if (allocChunk_ == 0 || allocChunk_->blocksAvailable_ == 0)
	{
		Chunks::iterator i = chunks_.begin();
		for (;; ++i)
		{
			if (i == chunks_.end())
			{
				chunks_.reserve(chunks_.size() + 1);
				Chunk newChunk;
				newChunk.Init(blockSize_, numBlocks_);
				chunks_.push_back(newChunk);
				allocChunk_ = &chunks_.back();
				deallocChunk_ = &chunks_.back();
				break;
			}
			if (i->blocksAvailable_ > 0)
			{
				allocChunk_ = &*i;
				break;
			}
		}
	}
	assert(allocChunk_ != 0);
	assert(allocChunk_->blocksAvailable_ > 0);
	return allocChunk_->Allocate(blockSize_);
}

void FixedAllocator::Deallocate(void* p) {
	unsigned char* deallocateP = (unsigned char*)p;

	// If the pointer is in the current chunk
	if (deallocChunk_->pData_ - deallocateP >= 0 && deallocateP <= deallocChunk_->pData_ + blockSize_ * numBlocks_)
	{
		// Delegate the deallocation to the chunk with the correct block size
		deallocChunk_->Deallocate(deallocateP, blockSize_);
	}
	else
	{
		// Start from deallocChunk and go both ways
		// Pointer Before and Pointer After
		Chunk* pb, * pa;

		// Check if dealloc chunk is already at the limit of the vector, 
		pb = deallocChunk_;
		pa = deallocChunk_;

		// Keep checking until we reach the end of the vector in both sides
		while (pb != &chunks_.front() || pa != &chunks_.back())
		{
			// Decrement pointer if begin of vector hasn't been reached yet
			if (pb != &chunks_.front())
			{
				--pb;
			}
			// Increment pointer if end of vector hasn't been reached yet
			if (pa != &chunks_.back())
			{
				++pa;
			}
			// Check if the chunk before is the correct chunk
			
			if (pb->pData_ - deallocateP >= 0 && deallocateP <= pb->pData_ + blockSize_ * numBlocks_)
			{
				// If so, call the deallocation of the chunk before
				pb->Deallocate(deallocateP, blockSize_);
				deallocChunk_ = pb;
				return;
			}
			// Check if the chunk after is the correct chunk
			else if (pa->pData_ - deallocateP >= 0 && deallocateP <= pa->pData_ + blockSize_ * numBlocks_)
			{
				// If so, call the deallocation of the chunk after
				pa->Deallocate(deallocateP, blockSize_);
				deallocChunk_ = pa;
				return;
			}

			
		}
	}
}


size_t FixedAllocator::GetBlockSize() {
	// Return the block size of the Fixed Allocator
	return blockSize_;
}
