#pragma once

#include <vector>

#include "BOA_Block.h"
#include <RedBlackTree/RBTree.h>

enum FitType
{
	BEST_FIT,
	WORST_FIT,
	FIRST_FIT
};

class BigObjectAllocator
{
public:

	/*
	* Initializes the Allocator
	* 
	* @param sizeAlloc The size allocated
	* @param startingObjectSize The minimum size of the object to be allocated, any less than this gets redirected to the SOA
	* @param fitType The type of fit of the allocator
	*/
	BigObjectAllocator(size_t sizeAlloc, std::size_t startingObjectSize, FitType fitType = BEST_FIT);

	/*
	* Destroys the Allocator
	*/
	~BigObjectAllocator();

	/*
	* Returns a void pointer to the chunk just allocated
	* 
	* @param numBytes The quantity to be allocated 
	* 
	* @return a void pointer to the data
	*/
	void* Allocate(std::size_t numBytes);

	/*
	* Deallocates the pointer passed and puts the free block in the RBTree
	* 
	* @param p The pointer to be deallocated
	* @param size The size that needs to be deallocated
	*/
	void Deallocate(void* p, std::size_t size);

	/*
	* Joins near block of the block just passed to merge all of them into one
	* 
	* @param block The block just deallocated
	*/
	void joinNearBlocks(BOA_Block* block);



private:
	size_t sizeAlloc_;
	size_t startingObjectSize_;
	FitType fitType_;

	unsigned char* tail_;

	RBTree* rbTree;
};
