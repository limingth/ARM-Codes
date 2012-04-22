#ifndef __NAND_FLASH__
#define __NAND_FLASH__

void nand_init(void);
void nand_reset(void);
int nand_readpage(unsigned int block, unsigned int page, unsigned char *buf);
int nand_program(unsigned int block, unsigned char *buf, unsigned int page);
void nand_blockerase(unsigned int block);

#endif