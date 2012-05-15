#include "stdio.h"
#include "lib.h"
#include "xmodem.h"
#include "nand.h"

int help(int argc, char * argv[])
{
	printf("help usage:\n");
	printf("md - memory dispaly\n");
	printf("mw - memory write\n");
	printf("loadb - load .bin file from uart\n");
	printf("go - go to address\n");
	printf("loadx - load .bin file using xmodem\n");
	printf("nand - nand read/write\n");
	printf("bootm - boot linux kernel\n");

	return 0;
}

int md(int argc, char * argv[])
{	
	int * p = (int *)0;
	int i, j;
	
	if (argc >= 2)
		p = (int *)atoi(argv[1]);
		
	for (j = 0; j < 16; j++)
	{	
		printf("%x: ", (int)p);
		for (i = 0; i < 4; i++)
			printf("%x ", *p++);	
		printf("\n");
	}
		
	return 0;
}

// mw 0x0 0x1122
int mw(int argc, char * argv[])
{	
	int * p = (int *)0;
	int v = 0;
	
	if (argc >= 2)
		p = (int *)atoi(argv[1]);
		
	if (argc >= 3)
		v = atoi(argv[2]);
		
	*p = v;
	
	return 0;
}


#define DOWN_BIN_ADDR	(0xD0022000)	// 0xD0020000 + 8K as .bin file load addr

int loadb(int argc, char * argv[])
{
	int i = 0;
	int size = 0;
	char * p = (char *)DOWN_BIN_ADDR;
	
	printf("load bin file to address 0x%x\n", DOWN_BIN_ADDR);
		
	if (argc >= 2)
		size = atoi(argv[1]);
	
	for (i = 0; i < size; i++)
		*p++ = getchar();
	
	printf("load finished! \n");
	
	return 0;
}

int loadx(int argc, char *argv[])
{	
	char * p = (char *)DOWN_BIN_ADDR;
	
	if (argc >= 2)
		p = (char *)atoi(argv[1]);
	
	printf("load bin file by xmodem to address 0x%x\n", (int)p);		
			
	xmodem_recv(p);
	
	printf("loadx finished! \n");

	return 0;
}

int go(int argc, char * argv[])
{
	int addr = DOWN_BIN_ADDR;
	void (*fp)(void);
	
	if (argc >= 2)
		addr = atoi(argv[1]);
	
	printf("go to address 0x%x\n", addr);

	fp = (void (*)(void))addr;
	
	fp();
	
	return 0;
}


/* load from nand_flash */
int nand(int argc, char *argv[])
{
	int nand_addr, sdram_addr;
	unsigned int size;
	
	if (argc < 5)
		return 0;
		
	sdram_addr = atoi(argv[2]);
	nand_addr = atoi(argv[3]);
	size = atoi(argv[4]);

	printf("sdram 0x%x, nand 0x%x, size 0x%x\n", sdram_addr, nand_addr, size);

	if (strcmp(argv[1], "read") == 0)
		nand_read(sdram_addr, nand_addr, size);

	if (strcmp(argv[1], "write") == 0)
		nand_write(sdram_addr, nand_addr, size);

	printf("nand %s finished!\n", argv[1]);
	return 0;
}


typedef unsigned int __u32;
typedef unsigned char __u8;
typedef unsigned short __u16;

struct tag_header {
	__u32 size;
	__u32 tag;
};
struct tag_core {
	__u32 flags;		/* bit 0 = read-only */
	__u32 pagesize;
	__u32 rootdev;
};
struct tag_mem32 {
	__u32	size;
	__u32	start;	/* physical start address */
};

struct tag_initrd {
	__u32 start;	/* physical start address */
	__u32 size;	/* size of compressed ramdisk image in bytes */
};

struct tag_cmdline {
	char	cmdline[1];	/* this is the minimum size */
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
		struct tag_mem32	mem;
		struct tag_initrd	initrd;
		struct tag_cmdline	cmdline;
	} u;
};

#define tag_next(t)	((struct tag *)((__u32 *)(t) + (t)->hdr.size))
#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)

/* The list must start with an ATAG_CORE node */
#define ATAG_CORE	0x54410001
/* it is allowed to have multiple ATAG_MEM nodes */
#define ATAG_MEM	0x54410002
/* describes where the compressed ramdisk image lives (physical address) */
#define ATAG_INITRD2	0x54420005
/* The list ends with an ATAG_NONE node. */
#define ATAG_NONE	0x00000000
/* command line: \0 terminated string */
#define ATAG_CMDLINE	0x54410009

const char cmd[] = "root=/dev/mtdblock4 console=ttySAC0,115200 init=/linuxrc lcd=H43";

void init_taglist(int addr)
{
	struct tag * p;
	int i;
	
	p = (struct tag*) addr;
	p->hdr.tag  =  ATAG_CORE;
	p->hdr.size = tag_size(tag_core);
	p->u.core.flags = 1;
	p->u.core.pagesize = 4096;
	p->u.core.rootdev = 0x00000000;

	p = tag_next(p);
	p->hdr.tag = ATAG_CMDLINE;
	p->hdr.size =  (sizeof (cmd) + sizeof(struct tag_header) + 3) >>2;	
	for(i=0; i< sizeof (cmd); i++)	
		p->u.cmdline.cmdline[i] = cmd[i];

	p = tag_next(p);
	p->hdr.tag = ATAG_MEM;
	p->hdr.size = tag_size(tag_mem32);
	p->u.mem.size = 256*1024*1024;
	p->u.mem.start = 0x20000000;

	p = tag_next(p);
	p->hdr.tag = ATAG_NONE;
	p->hdr.size = 0;
}

#define LINUX_BOOT_ADDR		0x20008000
int bootm(int argc, char * argv[])
{
	int addr = LINUX_BOOT_ADDR;
	void (*fp)(int, int, int);
	int taglist_mem_address = 0x20000100;
	
	printf("loading linux from NandFlash 4M to SDRAM 0x%x...\n", LINUX_BOOT_ADDR);

	// copy kernel from flash 4M to sdram 0x20008000 with size of 8M
	nand_read(LINUX_BOOT_ADDR, 0x400000, 0x800000);

	fp = (void (*)(int, int, int))addr;
	
	init_taglist(taglist_mem_address);

	printf("lumit boot linux now...\n");
	fp(0, 3466, taglist_mem_address);
	
	return 0;
}

int command_do(int argc, char * argv[])
{
	if (argc == 0)
		return -1;

	printf("command do <%s> \n", argv[0]);

	if (strcmp(argv[0], "help") == 0)
		help(argc, argv);
		
	if (strcmp(argv[0], "md") == 0)
		md(argc, argv);

	if (strcmp(argv[0], "mw") == 0)
		mw(argc, argv);
	
	if (strcmp(argv[0], "loadb") == 0)
		loadb(argc, argv);

	if (strcmp(argv[0], "go") == 0)
		go(argc, argv);
		
	if (strcmp(argv[0], "loadx") == 0)
		loadx(argc, argv);

	if (strcmp(argv[0], "nand") == 0)
		nand(argc, argv);

	if (strcmp(argv[0], "bootm") == 0)
		bootm(argc, argv);

	return 0;
}
