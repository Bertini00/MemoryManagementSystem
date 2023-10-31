#include "BigObjectAllocator.h"


BigObjectAllocator::BigObjectAllocator(size_t sizeAlloc, std::size_t startingObjectSize, FitType fitType) :sizeAlloc_(sizeAlloc), startingObjectSize_(startingObjectSize), fitType_(fitType)
{
	rbTree = new RBTree();

	BOA_Block* block = new BOA_Block(new unsigned char[sizeAlloc]);
	tail_ = block->getPointerToData();
	tail_ += sizeAlloc - block->getHeaderSize();

	rbTree->Insert(sizeAlloc - block->getHeaderSize(), block->getPointerToData());
	
}

BigObjectAllocator::~BigObjectAllocator() {
}

void* BigObjectAllocator::Allocate(std::size_t numBytes) {
	switch (fitType_)
	{
	case BEST_FIT:
		unsigned char* currentBest = rbTree.


		break;
	case WORST_FIT:
		break;
	case FIRST_FIT:
		break;
	default:
		break;
	}


	return new int;
}

void BigObjectAllocator::Deallocate(void* p, std::size_t size) {
	return;
}

void BigObjectAllocator::joinNearBlocks(BOA_Block* block) {
	
}