#if 0
#include <stdarg.h>
#else
// my own definition of va_xxx

typedef int *	va_list;
#define va_start(ap, fmt)	ap = (va_list)&fmt
#define va_arg(ap, type)	(type)(*++ap)
#define va_end(ap)			ap = (void *)0

#endif

#include "stdio.h"

char * itoa(int val, char * buf)
{
	int i = 0;
	int j = 0;
	
	do
	{
		buf[i++] = val % 10 + '0';
		val /= 10;
	} while (val);
	
	buf[i] = '\0';
	
	// reverse buf string
	for (j = 0; j < i / 2; j++)
	{
		// buf[0] <--> buf[i-1]
		// buf[j] <--> buf[i-1-j]
		char tmp;
		tmp = buf[j];
		buf[j] = buf[i-1-j];
		buf[i-1-j] = tmp;
	}
	
	return buf;
}

void print_hex_r(int val)
{
	char * str = "0123456789ABCDEF";
	static int count = 1;
	if(count == 9)
	{
		count = 1;
		return;
	}	
	count++;
	print_hex_r(val >> 4);
	putchar(str[val & 0xf]);
}

void print_hex(int val)
{
	int i = 0;
	int tmp = 0;
	//char * str = "0123456789abcdef";
	
	for(i = 7; i >= 0; i--)
	{
		tmp = (val >> (i*4)) & 0xf;
		if(tmp < 10)	
			putchar(tmp + '0');	
		else 
			putchar(tmp - 10 + 'A');
		// putchar(str[tmp]);
	}
	
	return;
}

void xtoa(int value, char *buf)
{
	char *str = "0123456789abcdef";
	//static char str[] = "0123456789abcdef";
	int mask = 0x0f;
	int flag = 0 ;
	int i = 0;
	int j = 0;
	for(i = 7; i >= 0; i--)
	{
		flag = (value &(mask << (4*i)))>>(4*i);
		buf[j++] = str[flag];
	}
	buf[j] = '\0';
}

void print_int(int val)
{
	static int counter = 1;
	
	if (val == 0 && counter > 1)
	{	
		counter = 1;
		return;
	}
	
	counter++;
	
	print_int(val / 10);	
	
	putchar(val % 10 + '0');
}

int printf(const char * format, ...)
{
	char c;
	char ch;
	int value;
	//char buf[100];	
	char *p;
	va_list ap;
//	int * ap;
	
	va_start(ap, format);
//	ap = (int *)&format;
		
	while ((c = *format++) != '\0')
	{
		switch (c)
		{			
			case '%':
				// deal with %c, %s
				c = *format++;
				switch (c)
				{						
					case 'c':											
						ch = va_arg(ap, int);
					//	ap++;
					//	ch = (char)(*ap);
						putchar(ch);
						break;						
					case 's':
						p = va_arg(ap, char *);
					//	ap++;
					//	p = (char *)(*ap);
						puts(p);
						break;
					case 'd':
						value = va_arg(ap, int);
					//	itoa(value, buf);
					//	puts(buf);
						print_int(value);
						break;
					case 'x':
						value = va_arg(ap, int);
						//print_hex(value);
						print_hex_r(value);
						break;
					case '%':
						putchar(c);
						break;
					default:
						break;
				}
				break;
			default:
				putchar(c);	
				break;
		}
	}
	
	//va_end(ap);

	return 0;
}