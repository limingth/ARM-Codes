#include "210sdhc.h"
#include "uart.h"
#include "stdio.h"
#include "fat.h"

U8 SDHC_Init(void);
U8 SDHC_ReadBlocks(U32 uStBlock, U16 uBlocks, U32 uBufAddr);

/*
void delay(void)
{
	int i;
	
	for (i = 0; i < 0x10000; i++)
		;
}
*/

char buff[2048];

void printblock(int id)
{
	int * p;
	int j;

	SDHC_ReadBlocks(id, 1, (unsigned int)buff);

	printf("\n");
	p = (int *)buff;
	for (j = 0; j < 32/4; j++)
	{
		printf("%x ", *(p+j));
		
		if ( j % 4 == 3 )
			printf("\n");
	}
	printf("\n");
}

unsigned long	block_read(int dev, unsigned long start, unsigned long blkcnt, void *buffer)
{
	int ret;
	printf("<main> block_read: start = %d, cnt = %d\n", start, blkcnt);
	ret = SDHC_ReadBlocks(start, (short)blkcnt, (int)buffer);
	printf("ret: %d\n", ret);
	return ret;
}

int mymain(void)
{
	//block_dev_desc_t *dev_desc = NULL;
	block_dev_desc_t dev_desc;
	
	int dev=0;
	int part=1;
	
	int id;
	char buf[128];
	
	puts("init begin\r\n");
//	uart_init();
	SDHC_Init();
	puts("init over\r\n");

	printblock(0);
	printblock(1);
	
	dev_desc.block_read = block_read;
	
	if (fat_register_device(&dev_desc, part) != 0) {
		printf("\n** Unable to use %s %d:%d for fatload **\n",
			"FAT", dev, part);
		return 1;
	}
	
	id = file_fat_detectfs();
	printf("file_fat_detectfs 0x%x\n", id);
	
//	file_fat_ls("/");
//	file_fat_ls("/1.wav");
	file_fat_read("/1.wav", buf, 64);
	printf("1.wav %s\n", buf);
//	file_fat_ls("/images");
	file_fat_read("/test.txt", buf, 64);
	printf("test.txt %s\n", buf);

	file_fat_read("/images/test2.txt", buf, 64);
	printf("test2.txt %s\n", buf);
		
	puts("game over");
	
	//SDHC_ReadBlocks();
	//SDHC_WriteBlocks();
	//SDHC_CloseMedia();

	return 0;
}
