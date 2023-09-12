#include "FixedAllocator.h"
#include <assert.h>



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