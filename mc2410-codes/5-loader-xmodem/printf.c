#include "stdio.h"
#include <stdarg.h>

void itoa(unsigned int num, char *buf, int base)
{
	int i = 0;	
	static char hex[17] = "0123456789ABCDEF";

	if (base == 10)
	{
		// do not support base 10
		*buf = '\0';	
		return;
	}

	for (i = 0; i < 8; i++)
		buf[i] = hex[(num >> ((7-i)*4)) & 0xF];
	
	buf[i] = '\0';

	return;
}

int printf(const char *format, ...)
{
	char c, tmp;
	char buf[100];
	int value;
	va_list ap;

	va_start(ap, format);
	
	while ((c = *format++) != '\0')
	{
		if (c == '%')
		{
			tmp = *format;
			switch(tmp)
			{
			case 'c':
				putchar(va_arg(ap, int));
				format++;
				break;			
			case 'd':
				value = va_arg(ap, int);
				itoa(value, buf, 10);
				format++;
				break;		
			case 'x':
				value = va_arg(ap, int);
				itoa(value, buf, 16);
				puts(buf);
				format++;
				break;
			case 's':
				puts(va_arg(ap, char *));
				format++;
				break;
			case '%':
				putchar('%');
				format++;
				break;
			default:
				putchar(c);
			}
		}
		else
		{
			if (c == '\n')
				putchar('\r');
			putchar(c);
		}
	}
	
	va_end(ap);
	
	return 0;
}