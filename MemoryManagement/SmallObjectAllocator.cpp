#include "SmallObjectAllocator.h"
#include <algorithm>


SmallObjectAllocator::SmallObjectAllocator(unsigned char chunkSize, std::size_t maxObjectSize)
	: chunkSize_(chunkSize)
	, maxObjectSize_(maxObjectSize)
	, pLastAlloc_(nullptr)
	, pLastDealloc_(nullptr)
{}

void SmallObjectAllocator::Deallocate(void* p, size_t size) {

	// Check if the last dealloc pointer has the correct size
	if (pLastDealloc_->GetBlockSize() == size)
	{
		// In case it does, deallocate
		pLastDealloc_->Deallocate(p);
		return;
	}

	// Otherwise, get the pointer after and the pointer before
	FixedAllocator* pa, *pb;

	//Check if there are pointer before and after, avoid undefined behavior
	pa = pLastDealloc_;
	pb = pLastDealloc_;

	auto front = &pool_.front();
	auto back = &pool_.back();
	
	// And keep checking until we finish the whole vector
	while (pb != &pool_.front() || pa != &pool_.back())
	{

		// Decrement pointer if begin of vector hasn't been reached yet
		if (pb != &pool_.front())
		{
			--pb;
		}
		// Increment pointer if end of vector hasn't been reached yet
		if (pa != &pool_.back())
		{
			++pa;
		}

		// Deallocate if is the correct FixedAllocator
		if (pa->GetBlockSize() == size)
		{
			pa->Deallocate(p);
			pLastDealloc_ = pa;
			return;
		}
		// Deallocate if is the correct FixedAllocator
		if (pb->GetBlockSize() == size)
		{
			pb->Deallocate(p);
			pLastDealloc_ = pb;
			return;
		}
	}
}

void* SmallObjectAllocator::Allocate(std::size_t numBytes) {


	// Check if the numBytes to allocate exceeds the max number of bytes
	if (numBytes > maxObjectSize_)
	{
		// If it exceeds, call the new function
		//return SmallObjectAllocator::new(numBytes);
	}

	if (pool_.size()> 0) {

		// Check if the last alloc pointer has the correct size
		if (pLastAlloc_ != nullptr && pLastAlloc_->GetBlockSize() == numBytes)
		{
			// In case it does, allocate
			return pLastAlloc_->Allocate();
		}

		// Otherwise, get the pointer after and the pointer before
		// pa= pointer after
		// pb= pointer before
		FixedAllocator* pa, *pb;
		//Check if there are pointer before and after, avoid undefined behavior
		pa = pLastAlloc_;
		pb = pLastAlloc_;

		while (pb != &pool_.front() || pa != &pool_.back())
		{

			// Decrement pointer if begin of vector hasn't been reached yet
			if (pb != &pool_.front())
			{
				--pb;
			}
			// Increment pointer if end of vector hasn't been reached yet
			if (pa != &pool_.back())
			{
				++pa;
			}

			// Allocate if is the correct FixedAllocator
			if (pa->GetBlockSize() == numBytes)
			{
				return pa->Allocate();

			}
			// Allocate if is the correct FixedAllocator
			if (pb->GetBlockSize() == numBytes)
			{
				return pb->Allocate();
			}


			
		}
	}
	FixedAllocator newAlloc = FixedAllocator(numBytes, chunkSize_);

	pool_.reserve(pool_.size() + 1);
	pool_.push_back(newAlloc);
	pLastAlloc_ = &pool_.back();
	pLastDealloc_ = &pool_.back();
	return pLastAlloc_->Allocate();

}

void SmallObjectAllocator::Free()
{
	for (auto& alloc : pool_)
	{
		alloc.Free();
	}
}
