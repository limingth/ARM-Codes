#include "my_stdio.h"
#include "my_command.h"
#include "uart0.h"
#include "my_printf.h"


#define	MAX_CMD_LEN	128
#define	MAX_ARGS	MAX_CMD_LEN/4
typedef int (*cmdproc)(int argc, char *argv[]);
typedef struct {
	const char *cmd;
	const char *hlp;
	cmdproc proc;
}CMD_STRUC;


#define DOWN_ADDR 0x50008000

CMD_STRUC CMD_INNER[] =
				{ 
					{"help", "    show help",help},	
					{"go", "      go at your input,default is 0x50008000", go},
					{"dn", "      download the image binary from UART0", dn},	
			//		{"testled", "test the led's status", led_test},
			//		{"testbeep", "test the beep's status", beep_test},
					{NULL, NULL, NULL}
				};



/*
CMD_STRUC CMD_INNER[] =
				{ 
					{"help", "    show help",help},
					{"dn", "      download the image from UART0", uue_receive},	
					{"xdown", "   download the image use Xmodem protocol", xmodem_receive},
					{"go", "      go at your input,default is DOWN_ADDR", go},
					{"testled", "test the led's status", led_test},
					{"testseg7", "test the seg7's status", seg7_test},
					{"testbeep", "test the beep's status", beep_test},
					{"testdip4", "test the dip4's status", dip4_test},
					{"testtime0", "test the time0's status", timer0_on},
					{"testint0", "test the int0's status", int0_on},
					{"flashw",    "  write the image to flash", flashw},					
					{NULL, NULL, NULL}
				};
								*/
/************************************************/
int help(int argc, char *argv[])
{
	int i;	
	my_putchar('\r');
	for(i=0; CMD_INNER[i].cmd!=NULL; i++)
	{
		if(CMD_INNER[i].hlp!=NULL)
		{
			my_puts(CMD_INNER[i].cmd);
			my_puts(" ------ ");
			my_puts(CMD_INNER[i].hlp);
			my_putchar('\r');
		}
	}
	
	return 0;
}

/************************************************/
static void ParseArgs(char *cmdline, int *argc, char **argv)
{
#define STATE_WHITESPACE	0
#define STATE_WORD			1

	char *c;
	int state = STATE_WHITESPACE;
	int i;

	*argc = 0;

	if(my_strlen(cmdline) == 0)
		return;

	/* convert all tabs into single spaces */
	c = cmdline;
	while(*c != '\0')
	{
		if(*c == '\t')
			*c = ' ';
		c++;
	}
	
	c = cmdline;
	i = 0;

	/* now find all words on the command line */
	while(*c != '\0')
	{
		if(state == STATE_WHITESPACE)
		{
			if(*c != ' ')
			{
				argv[i] = c;		//½«argv[i]Ö¸Ïòc
				i++;
				state = STATE_WORD;
			}
		}
		else
		{ /* state == STATE_WORD */
			if(*c == ' ')
			{
				*c = '\0';
				state = STATE_WHITESPACE;
			}
		}
		c++;
	}
	
	*argc = i;
	
#undef STATE_WHITESPACE
#undef STATE_WORD
}
static int GetCmdMatche(char *cmdline)
{
	int i;	
	
	for(i=0; CMD_INNER[i].cmd!=NULL; i++)
	{
		if(my_strncmp(CMD_INNER[i].cmd, cmdline, my_strlen(CMD_INNER[i].cmd))==0)
			return i;
	}
	
	return -1;
}

static int ParseCmd(char *cmdline, int cmd_len)
{
	int i;
	int argc, num_commands;
	char *argv[MAX_ARGS];

	ParseArgs(cmdline, &argc, argv);

	/* only whitespace */
	if(argc == 0) 
		return 0;

	my_printf("\n\rargc = %d \n");
	for (i = 0; i < argc; i++)
	{
		my_printf("argv[%d] = %s \n", i, argv[i]);		
	}
		
	num_commands = GetCmdMatche(argv[0]);
	if(num_commands<0)
		return -1;
		
	if(CMD_INNER[num_commands].proc!=NULL)	
		CMD_INNER[num_commands].proc(argc, argv);
				
	return 0;			
}

void my_display(void)
{
	char prompt[] = "AKAE6410_Boot>";
	my_puts(prompt);
}

void my_shell(void)
{
	char s[100];
	int len;
	
//	uart0_init();
	
	while(1)
	{
		my_display();
		my_gets(s);
		len = my_strlen(s);
		//my_printf("%d",len);/////////////////////////////////
		if(ParseCmd(s,len)== -1) 
		{			
			my_puts("bad command, please enter \"help\" to check commands") ;
		}
		my_putchar('\r');
	}
}

int go(int argc, char *argv[])
{
	unsigned int addr;
	void (*fp)(void);
	
	if(argc > 1) 
		addr =  atox(argv[1]);
	else 
		addr = DOWN_ADDR;
		
	fp = (void (*)(void)) addr;
	my_putchar('\r');
	my_puts("go at address:");
	my_printf("0x%x ......",addr);
	my_putchar('\r');
	(*fp)();  
	
	return 0;
}

int dn(int argc, char *argv[])
{
	int i; char c;
	char * p = (char *)DOWN_ADDR;
	
	/* load led-blink.bin(116 bytes) from uart */
	for( i = 0; i < 116; i++ )
	{
		c = uart0_getchar();
		*p = c;
		p++;	
	}
	
	return 0;
}