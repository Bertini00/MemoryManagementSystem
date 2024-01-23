#pragma once


class Chunk
{
public:

	void Init(size_t blockSize, unsigned char blocks); 
	void* Allocate(size_t blockSize); 
	void Deallocate(void* p, size_t blockSize); 

	unsigned char* pData_; 
	unsigned char firstAvailableBlock_, blocksAvailable_;

	/*
	* Frees all the resources of the Chunk
	*/
	void Free();
};