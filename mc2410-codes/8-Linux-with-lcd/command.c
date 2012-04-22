#include "stdio.h"
#include "nand.h"
//#include <string.h>

#if 1
int help(int argc, char * argv[])
{
	printf("md - memory display\n");
	printf("mw - memory write\n");
	printf("loadb - load file to memory\n");
	printf("loadx - load file with Xmodem to memory\n");
	printf("go - set pc to addr\n");
	printf("nr - load from flash memory\n");
	printf("linux - boot linux kernel at 0x30008000 \n");
	return 0;
}
#endif

int atoi(char * buf)
{
	// "123" -> 123
	// "0x10" -> 16
	int num = 0;
	int radix = 10;
	
	if ((*buf == '0' ) && (*(buf + 1) == 'x')) {
		buf += 2;
		radix = 16;
	}
	
	while (*buf) {
		if (*buf <= '9') 
			num = num * radix + *buf - '0';
		else
			num = num * radix + *buf - 'a' + 10;
		buf++;
	}
	
	return num;
}

//#define LOAD_ADDR	0xC00	// 3k
#define LOAD_ADDR	0x31000000
#define LOAD_SIZE 	324

#if 1
int go(int argc, char * argv[])
{
	// go 0x0(addr) 
	int addr = LOAD_ADDR;
	void (*fp)(void);
	
	if (argc > 1) 	
		addr = atoi(argv[1]);
		
	fp = (void (*)(void))addr;
	
	fp();
	
	return 0;
}
#endif

#define COMMAND_LINE_SIZE 128

/* This is the old deprecated way to pass parameters to the kernel */
struct param_struct {
    union {
        struct {
            unsigned long page_size; /* 0 */
            unsigned long nr_pages; /* 4 */
            unsigned long ramdisk_size; /* 8 */
            unsigned long flags; /* 12 */
#define FLAG_READONLY 1
#define FLAG_RDLOAD 4
#define FLAG_RDPROMPT 8
            unsigned long rootdev; /* 16 */
            unsigned long video_num_cols; /* 20 */
            unsigned long video_num_rows; /* 24 */
            unsigned long video_x; /* 28 */
            unsigned long video_y; /* 32 */
            unsigned long memc_control_reg; /* 36 */
            unsigned char sounddefault; /* 40 */
            unsigned char adfsdrives; /* 41 */
            unsigned char bytes_per_char_h; /* 42 */
            unsigned char bytes_per_char_v; /* 43 */
            unsigned long pages_in_bank[4]; /* 44 */
            unsigned long pages_in_vram; /* 60 */
            unsigned long initrd_start; /* 64 */
            unsigned long initrd_size; /* 68 */
            unsigned long rd_start; /* 72 */
            unsigned long system_rev; /* 76 */
            unsigned long system_serial_low; /* 80 */
            unsigned long system_serial_high; /* 84 */
            unsigned long mem_fclk_21285; /* 88 */
        } s;
        char unused[256];
    } u1;
    union {
        char paths[8][128];
        struct {
            unsigned long magic;
            char n[1024 - sizeof(unsigned long)];
        } s;
    } u2;
    char commandline[COMMAND_LINE_SIZE];
};
 
#define LINUX_KERNEL_OFFSET	0x8000
#define LINUX_PARAM_OFFSET	0x100
#define LINUX_PAGE_SIZE		0x1000	// 4K
#define LINUX_PAGE_SHIFT	12
#define LINUX_ZIMAGE_MAGIC	0x016f2818
#define DRAM_SIZE		(0x100000*64)	// 64M
char * linux_cmd = "root=/dev/mtdblock3 "
				"init=/linuxrc console=ttyS0,115200 "
			//	"cs89x0_media=rj45 ";
				"ip=192.168.0.101:192.168.0.100:<NULL>:255.255.255.0::eth0:on";
 
int memset0(char * p, int v, int size)
{
	while (size--)
		*p++ = v;
	
	return 0;
}

int memcpy0(char * s1, char * s2, int size)
{
	int i = 0;
	
	for (i = 0; i < size; i++)
		*s1++ = *s2++;
	
	return 0;
}

int strlen0(char * s)
{
	int len = 0;
	
	while (*s++)
		len++;
		
	return len;
}

void setup_linux_param(unsigned long param_base)
{
	struct param_struct *params = (struct param_struct *)param_base; 

	memset0((char *)params, 0, sizeof(struct param_struct));
	
	/* ²ÎÊý´«µÝ */
	params->u1.s.page_size = LINUX_PAGE_SIZE;
	params->u1.s.nr_pages = (DRAM_SIZE >> LINUX_PAGE_SHIFT);

	/* set linux command line */
//	linux_cmd = get_linux_cmd_line();
	memcpy0(params->commandline, linux_cmd, strlen0(linux_cmd) + 1);
}

int linux(int argc, char * argv[])
{
	int taglist = 0x30000100;
	void (*fp)(int, int, int);
	
	fp = (void (*)(int, int, int))0x30008000;
	
	puts("linux go to 0x30008000\n");
	
	setup_linux_param(taglist);
	fp(0, 193, taglist);
	
	return 0;
}

#if 1
int loadx(int argc, char * argv[])
{
	// loadb 0x0(addr) 100(size)
	int addr = LOAD_ADDR;
	int i;
	
	if (argc > 1)
		addr = atoi(argv[1]);
	
	for (i = 0; i < 0x900000; i++)
		if ((i & 0xfffff) == 0)
			printf("%d ", i>>20);
			
#define	SOH 0x01
#define	EOT 0x04
#define	ACK 0x06
#define	NAK 0x15
#define	CAN 0x18 

	putchar(NAK);
	while (getchar() != EOT)
	{
		// get 131 bytes
		// omit 2 bytes (serial num)
		getchar();
		getchar();
		
		// get 128 bytes (data)
		for (i = 0; i < 128; i++)
			*(char *)(addr++) = getchar();

		// omit 1 byte (parity)
		getchar();

		// send ACK
		putchar(ACK);
	}
	putchar(ACK);
	
	return 0;
}

int loadb(int argc, char * argv[])
{
	// loadb 0x0(addr) 100(size)
	int addr = LOAD_ADDR;
	int size = LOAD_SIZE;
	int i;
	
	if (argc > 2) 
	{
		addr = atoi(argv[1]);
		size = atoi(argv[2]);
	}
	
	for (i = 0; i < size; i++)
		*(char *)(addr + i) = getchar();		
		
	return 0;
}	

#endif

#define NAND_ADDR	0x1      
#define BUF_ADDR	0x32000000

int nr(int argc, char * argv[])
{
	int flash_addr = NAND_ADDR;
	char * buf = (char *)BUF_ADDR;
	int size = 0x1000;
	
	if (argc > 3)
	{
		flash_addr = atoi(argv[1]);
		buf = (char *)atoi(argv[2]);
		size = atoi(argv[3]);
	}
	
	nand_read(flash_addr, buf, size);
	printf("nandread 0x%x to buf 0x%x (size = 0x%x) OK!\n", flash_addr, (int)buf, size);
	
	return 0;
}

#if 1
int mw(int argc, char * argv[])
{
	// mw 0x0 0x1234
	int addr = 0;
	int val = 0;
	
	if (argc < 3) 
		return 0;
		
	addr = atoi(argv[1]);	
	val = atoi(argv[2]);
	
	if (argc == 3)
		*(int *)addr = val;
	else
		*(char *)addr = val;
	
	return 0;
}
#endif

void autoplay(void)
{
#if 0	
	char * go_argv[2] = {"go", "0x31000000"};
	
	// flash 1M: -> SDRAM 0x32000000
	nand_read(0x100000, (char *)0x32000000, 0x100000);
	
	// flash 2M: -> SDRAM 0x33000000
	nand_read(0x200000, (char *)0x33000000, 0x100000);	
	
	// flash 512K: -> wavplayer.bin
	nand_read(0x80000, (char *)0x31000000, 0x80000);
	
	// go
	//((void (*)(void)0x31000000)();	
	go(2, go_argv);
#endif

	// load linux kernel from flash 1M: -> SDRAM 0x30008000 (size = 2M)
	printf("load linux kernel from flash 0x30000: -> SDRAM 0x30008000 (size = 2M) \n");
	nand_read(0x30000, (char *)0x30008000, 0x200000);	
	
	// boot linux kernel
	printf("booting linux kernel ... \n");
	linux(0, 0);
	
	// load rootfs from flash 4M: -> SDRAM 0x30800000 (size = 6M)	
	//printf("load rootfs from flash 4M: -> SDRAM 0x30800000 (size = 6M) \n");
	//nand_read(0x400000, (char *)0x30800000, 0x600000);	
}

#if 0
int play(int argc, char * argv[])
{
	autoplay();
	
	return 0;
}
#endif

#if 1
int md(int argc, char * argv[])
{
	int addr = 0;
	int * p;
	int i, j;

	if (argc > 1)
		addr = atoi(argv[1]);
	
	p = (int *)addr;
	
	for (i = 0; i < 16; i++)
	{	
		printf("%x: ", (int)p);
		for (j = 0; j < 4; j++)	
			printf("%x ", *p++);			
		printf("\n");
	}
	return 0;
}
#endif

int strcmp(const char * s1, const char * s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return 0;
		s1++;
		s2++;
	}
	
	return 	*s1 - *s2;
}

struct CMD
{
	char name[8];
	int (*fp)(int argc, char *argv[]);
};

struct CMD command[] =
{
	{"help", help},
	{"md", md},
	{"mw", mw},
	{"loadb", loadb},
	{"go", go},
	{"loadx", loadx},
	{"nr", nr},
//	{"play", play},
	{"linux", linux},
};

int command_do_main(int argc, char * argv[])
{
	int cmd_num;
	int i;
	
	cmd_num = sizeof(command) / sizeof(struct CMD);
	for (i = 0; i < cmd_num; i++)
	{	
		if (strcmp(argv[0], command[i].name) == 0)
		{
			command[i].fp(argc, argv);
			return 0;
		}
	}
	
	printf("unknown command <%s>\n", argv[0]);
	
#if 0
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
#endif	
	return 0;
}