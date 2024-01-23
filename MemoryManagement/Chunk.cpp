#include "Chunk.h"
#include <malloc.h>

void Chunk::Init(size_t blockSize, unsigned char blocks)
{
	pData_ = (unsigned char*)malloc(blockSize * blocks);
	firstAvailableBlock_ = 0;
	blocksAvailable_ = blocks;
	unsigned char i = 0;
	unsigned char* p = pData_;
	for (; i != blocks; p += blockSize)
	{
		*p = ++i;
	}
}

void* Chunk::Allocate(size_t blockSize) {
	if (!blocksAvailable_)
		return 0;

	unsigned char* pResult = pData_ + (firstAvailableBlock_ * blockSize);
	// Update firstAvailableBlock_ to point to the next block 
	firstAvailableBlock_ = *pResult;
	--blocksAvailable_;

	return pResult;
}

void Chunk::Deallocate(void* p, size_t blockSize) {

	// Cast to unsigned char so we can write on the first byte
	unsigned char* deallocatedPointer = (unsigned char*)p;

	// Calculate the position of the soon to be first available block
	unsigned char fab = (deallocatedPointer - pData_) / blockSize;

	// Write the previoud available block into the block we are freeing
	*deallocatedPointer = firstAvailableBlock_;

	// Swap the first available block with the one we are freeing
	firstAvailableBlock_ = fab;

	// Update the blocks available
	++blocksAvailable_;

	return;
}

void Chunk::Free()
{
	free(pData_);
}
