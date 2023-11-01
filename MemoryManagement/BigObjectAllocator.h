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

	/*
	Keys of the Node contains only the size of the memory available
	Value of the Node contains a pointer, header INCLUDED
	*/
	RBTree* rbTree;
};


/*
Gestisco la memoria come raw memory, creo un Boa Block iniziale con la malloc e poi gestisco tutta la memoria tramite puntatori
Il BOA Block è solamente l'header che viene utilizzato solamente nell'init dell'allocator

Esempio:
chiedo 200 byte, l'rb tree ha un blocco da 1024, tolgo il blocco da 1024 avanzo il puntatore di 200 + header, setto l'header del nuovo blocco e reinserisco il blocco restante
in seguito ritorno il puntatore al blocco libero da 200

*/