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
		/*
			Problem:
			We either linear search until we find the deallocChunk and then we go both ways (possible but slower)
			Or we find a way to get an iterator to the deallocChunk in constant time (don't know if possible but faster)
		*/
	}
}