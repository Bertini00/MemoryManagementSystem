#include "BOA_Block.h"


BOA_Block::BOA_Block(unsigned char* pData): pData_(pData), next_(nullptr), prev_(nullptr) {}
BOA_Block::BOA_Block(unsigned char* pData, unsigned char* next): pData_(pData), next_(next), prev_(nullptr) {}
BOA_Block::BOA_Block(unsigned char* pData, unsigned char* next, unsigned char* prev): pData_(pData), next_(next), prev_(prev) {}

size_t BOA_Block::getHeaderSize() {
	return sizeof(prev_) + sizeof(next_);
}