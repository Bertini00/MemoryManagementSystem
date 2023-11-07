#include "BigObjectAllocator.h"
#include <malloc.h>

BigObjectAllocator::BigObjectAllocator(size_t sizeAlloc, std::size_t startingObjectSize)
	: sizeAlloc_(sizeAlloc)
	, startingObjectSize_(startingObjectSize)
{
	// Create RBTree
	rbTree = (RBTree*)malloc(sizeof(RBTree));
	new(rbTree) RBTree();

	// Calculate the effective size with header included
	size_t effectiveSize = sizeAlloc + sizeof(BOA_Block);

	// Create memory
	unsigned char* pointer = (unsigned char* )malloc(effectiveSize);

	// Set the tail
	tail_ = pointer + effectiveSize;

	// Add header to memory just created
	new(pointer) BOA_Block(tail_);

	// Insert the memory block in the rb tree
	rbTree->Insert(sizeAlloc, pointer);
}

BigObjectAllocator::~BigObjectAllocator()
{
	// destructor and free because the tree was created with malloc
	rbTree->~RBTree();
	free(rbTree);
}

void* BigObjectAllocator::Allocate(std::size_t numBytes)
{
	// Get the nearest node from the tree
	RBNode* node = rbTree->LookUpAtLeast(numBytes);

	unsigned char* result = nullptr;

	// Check if there is a space for the numBytes requested
	if (node != nullptr)
	{
		// Get header of resulting pointer
		BOA_Block* headerResult = (BOA_Block*)node->value;

		// Get the correct pointer to data to return without the header 
		result = (unsigned char* )node->value + sizeof(BOA_Block);

		// Get the pointer to the next block header included
		unsigned char* tmp = result + numBytes;


		// Add the header to the new block
		BOA_Block * block = new(tmp) BOA_Block(((BOA_Block*)(node->value))->next_, (unsigned char*)node->value);

		// Set next to the result pointer header
		headerResult->next_ = tmp;

		// Set available to false
		headerResult->available = 0;

		// Remove the block from the tree
		rbTree->Delete(node->key, node->value);

		// Insert the new block 
		rbTree->Insert(block->next_ - tmp - sizeof(BOA_Block), tmp);
	}
	return result;
}

void BigObjectAllocator::Deallocate(void* p, std::size_t size) {

	// Cast to unsigned to write in it
	unsigned char* pointer = (unsigned char *)p;

	// Get the pointer with the header
	pointer -= sizeof(BOA_Block);

	// Get the header
	BOA_Block* header = (BOA_Block*)pointer;

	JoinNearBlocks(header);
}

void BigObjectAllocator::JoinNearBlocks(BOA_Block* block) {

	// Join this with next
	if (block->next_ != tail_)
	{
		BOA_Block* next = (BOA_Block*)block->next_;
		
		if (next->available)
		{
			block->next_ = next->next_;
			rbTree->Delete(next->next_ - (unsigned char*)next - sizeof(BOA_Block), next);
		}
	}

	// Join this with prev
	if (block->prev_ != nullptr)
	{
		BOA_Block* prev = (BOA_Block*)block->prev_;
		if (prev->available)
		{
			prev->next_ = block->next_;
			rbTree->Delete(block - prev - sizeof(BOA_Block), prev);
		}
	}
	rbTree->Insert(block->next_ - (unsigned char*)block - sizeof(BOA_Block), block);
}