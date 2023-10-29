#pragma once

class BOA_Block
{
public:
	BOA_Block(unsigned char* pData);
	BOA_Block(unsigned char* pData, unsigned char* next);
	BOA_Block(unsigned char* pData, unsigned char* next, unsigned char* prev);
	~BOA_Block();

	virtual size_t getHeaderSize();

private:
	unsigned char* pData_;
	unsigned char* prev_;
	unsigned char* next_;
};

// Potrebbe essere necessario una funzione per calcolare la size of dell'header
// pData dovrebbe puntare all'inizio del blocco, header incluso, quindi quando si alloca bisogna considerare la dimensione dell'header
// e avanzare di quella dimensione per non sovrascriverlo