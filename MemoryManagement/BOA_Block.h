#pragma once

class BOA_Block
{
public:
	/*
	* Initializes a Big object allocator block
	* 
	* @param pData The pointer to the start of the block, header included
	* @param next The pointer to the next block, nullptr by default
	* @param prev The pointer to the previous block, nullptr by default
	*/
	BOA_Block(unsigned char* pData, unsigned char* next = nullptr, unsigned char* prev = nullptr);
	
	/*
	* Returns the header size of the block
	* 
	* @return Header size of the block
	*/
	size_t getHeaderSize();

	/*
	* Returns a pointer to the block without the header
	* 
	* @return the pointer to the start of the data
	*/
	unsigned char* getPointerToData();

private:

	unsigned char* pData_;

	unsigned char* prev_;
	unsigned char* next_;
};
