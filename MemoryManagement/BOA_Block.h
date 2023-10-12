#pragma once

class BOA_Block
{
public:
	BOA_Block(unsigned char* pData);
	BOA_Block(unsigned char* pData, unsigned char* next);
	BOA_Block(unsigned char* pData, unsigned char* next, unsigned char* prev);
	~BOA_Block();

private:
	unsigned char* pData_;
	unsigned char* prev_;
	unsigned char* next_;
};