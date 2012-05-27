typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;

U8 SDHC_Init(void);
U8 SDHC_WriteBlocks(U32 uStBlock, U16 uBlocks, U32 uBufAddr);
U8 SDHC_ReadBlocks(U32 uStBlock, U16 uBlocks, U32 uBufAddr);
