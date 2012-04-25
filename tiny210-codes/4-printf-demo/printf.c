#include "uart.h"
#include "lib.h"
//#include <stdarg.h>

typedef  int *		va_list;
#define va_start(ap, fmt)		ap = (va_list)&fmt
#define va_arg(ap, type)		(type)(*++ap)
#define va_end(ap)				ap = (void *)0

int myprintf(const char * format, ...)
{
	char c;
	
	va_list ap;
	
	va_start(ap, format);
	
	while ((c = *format) != '\0')
	{
		if (c == '%')
		{	
			char c_val;
			int i_val;
			char * p_val;
			char buf[10];
						
			c = *++format;
			
			switch (c)
			{						
				case 'c':			// 	myprintf("hell%c", 'o');
					c_val = va_arg(ap, int);		
					uart_putchar(c_val);					
					break;
				case 's':
					p_val = va_arg(ap, char *);
					puts(p_val);
					break;
				case 'x':
					i_val = va_arg(ap, int);
					putint_hex(i_val);
					break;
				case 'd':
					i_val = va_arg(ap, int);
					itoa(i_val, buf); 
					puts(buf);
					break;
				default:
					break;
			}
		}
		else
			putchar(c);
		
		format++;
	}
	
	va_end(ap);
	
	return 0;
}