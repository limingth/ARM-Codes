#include "stdio.h"

void putchar_hex(char c)
{
	char * hex = "0123456789ABCDEF";	// good
	//char hex[] = "0123456789ABCDEF";	bad!
	
	putchar(hex[(c & 0xf0)>>4]);
	putchar(hex[(c & 0x0f)>>0]);
	//putchar(' ');
}

void putint_hex(int a)
{
	putchar_hex( (a>>24) & 0xFF );
	putchar_hex( (a>>16) & 0xFF );
	putchar_hex( (a>>8) & 0xFF );
	putchar_hex( (a>>0) & 0xFF );
}

char * itoa(int a, char * buf)
{
	int num = a;
	int i = 0;
	int len = 0;
	
	do 
	{
		buf[i++] = num % 10 + '0';
		num /= 10;		
	} while (num);
	buf[i] = '\0';
	
	len = i;
	for (i = 0; i < len/2; i++)
	{
		char tmp;
		tmp = buf[i];
		buf[i] = buf[len-i-1];
		buf[len-i-1] = tmp;
	}
	
	return buf;	
}

#if 0
#include <stdarg.h>
#else
typedef int * va_list;
#define va_start(ap, A)		(ap = (int *)&(A) + 1)
#define va_arg(ap, T)		(*(T *)ap++)
#define va_end(ap)		((void)0)
#endif

int printf(const char * format, ...)
{
	char c;	
	va_list ap;
		
	va_start(ap, format);
	
	while ((c = *format++) != '\0')
	{
		switch (c)
		{
			case '%':
				c = *format++;
				
				switch (c)
				{
					char ch;
					char * p;
					int a;
					char buf[100];
									
					case 'c':
						ch = va_arg(ap, int);
						putchar(ch);
						break;
					case 's':
						p = va_arg(ap, char *);
						puts(p);
						break;					
					case 'x':
						a = va_arg(ap, int);
						putint_hex(a);
						break;		
					case 'd':
						a = va_arg(ap, int);
						itoa(a, buf);
						puts(buf);
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
	
	return 0;	
}

#if 0
	// get the stack address
	int addr = (int)&format;
	char * sp = (char *)addr;
	int i;
	
	putint_hex(addr);	// 0xF50 stack address
	
	// get the format string address
//	addr = (int)format;
//	putint_hex(addr);	// 0x110 bin rodata address

	for (i = 0; i < 30; i++)
		putchar_hex(*(sp+i));
#endif
