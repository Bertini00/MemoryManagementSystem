#include "BOA_Block.h"
#include <malloc.h>

BOA_Block::BOA_Block(unsigned char* next, unsigned char* prev) : next_(next), prev_(prev) 
{
	available = 1;
}


unsigned char* BOA_Block::getPointerToData() {
	return (unsigned char* )this + sizeof(BOA_Block);
}