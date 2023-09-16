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
	if (deallocateP - deallocChunk_->pData_ >= 0 && deallocateP - deallocChunk_->pData_ <= blockSize_ * numBlocks_)
	{
		// Delegate the deallocation to the chunk with the correct block size
		deallocChunk_->Deallocate(deallocateP, blockSize_);
	}
	else
	{
		// Start from deallocChunk and go both ways
		// Pointer Before and Pointer After
		Chunk* pb, * pa;
		pb = deallocChunk_ - 1;
		pa = deallocChunk_ + 1;

		// Keep checking until we reach the end of the vector in both sides
		while (pb != &*chunks_.begin() && pa != &*chunks_.end())
		{
			// Check if the chunk before is the correct chunk
			if (pb->pData_ - deallocChunk_->pData_ >= 0 && pb->pData_ - deallocChunk_->pData_ <= blockSize_ * numBlocks_)
			{
				// If so, call the deallocation of the chunk before
				deallocChunk_->Deallocate(pb->pData_, blockSize_);
				return;
			}
			// Check if the chunk after is the correct chunk
			else if (pa->pData_ - deallocChunk_->pData_ >= 0 && pa->pData_ - deallocChunk_->pData_ <= blockSize_ * numBlocks_)
			{
				// If so, call the deallocation of the chunk after
				deallocChunk_->Deallocate(pa->pData_, blockSize_);
				return;
			}

			// Decrement pointer if begin of vector hasn't been reached yet
			if (pb != &*chunks_.begin())
			{
				--pb;
			}
			// Increment pointer if end of vector hasn't been reached yet
			if (pa != &*chunks_.end())
			{
				++pa;
			}
		}
	}
}