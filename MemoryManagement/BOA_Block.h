#pragma once

/*
* Has functionality of header ONLY
*/
class BOA_Block
{
public:
	/*
	* Initializes a Big object allocator block
	* 
	* @param next The pointer to the next block, nullptr by default
	* @param prev The pointer to the previous block, nullptr by default
	*/
	BOA_Block(unsigned char* next = nullptr, unsigned char* prev = nullptr);
	

	/*
	* Returns a pointer to the block without the header
	* 
	* @return the pointer to the start of the data
	*/
	unsigned char* getPointerToData();

	unsigned char* prev_;
	unsigned char* next_;
	char available;

private:

};
