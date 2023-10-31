#include "BOA_Block.h"

BOA_Block::BOA_Block(unsigned char* pData, unsigned char* next, unsigned char* prev): pData_(pData), next_(next), prev_(prev) {}

size_t BOA_Block::getHeaderSize() {
	return sizeof(prev_) + sizeof(next_);
}

unsigned char* BOA_Block::getPointerToData() {
	return pData_ + BOA_Block::getHeaderSize();
}