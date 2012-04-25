#include <stdarg.h>
#include "uart0.h"

/*
#define va_start(ap,parmN) (void)(*(ap)=__va_start(parmN))
#define va_arg(ap,type) __va_arg(*(ap),type)
typedef int *va_list[1];
#define va_end(ap) ((void)(*(ap)=0))
*/
#define _putchar(x)	do{if (x=='\n') uart0_putchar('\r');uart0_putchar((char)(x));}while (0)


/*
 * transfer the num to 10 or 16 and ouput it
 */
void my_puts(const char *s)
{
	const char * p = s;
	char ch = *p;
	
	while(ch != '\0')
	{
		uart0_putchar('*');
		uart0_putchar(ch);
		uart0_putchar('+');
		p++;
		ch = *p;
	}
}

void output_int( unsigned int num, const int base )
{
	const char *digit = "0123456789ABCDEF";
	unsigned int buf[32];	
	int i = 0;	
	char ch;

	/*transfer the num to 10*/
	if (base == 10) 
	{	
		do
		{
			buf[i] = num % 10;
			num = num / 10;
			i++;
		} while ( num > 0 );
	}
	/*transfer the num to 16*/
	if (base == 16)
	{
		do
		{
			buf[i] = num % 16;
			num = num / 16;
			i++;
		} while ( num > 0 );
	}
	
	while ( --i >= 0 )	
	{
		ch = digit[buf[i]];
		_putchar( ch );
	}
}

int my_printf(const char *format, ...)
{
	va_list unnamed_p;
	/* point to named argument */
	char *p, *sval;
	unsigned int value_i;
	unsigned char value_c;
	
	/* unnamed_p point to first unnamed argument */
	va_start( unnamed_p, format);

	for ( p=(char *)format; *p!='\0'; p++ )
	{
		if ( *p != '%' )
		{
			_putchar( *p );
			continue;
		}

		switch ( *++p )
		{
		case 'd':
			value_i = va_arg( unnamed_p, unsigned int );		
			output_int( value_i, 10 );
			break;

		case 's':
			for (sval = va_arg(unnamed_p, char*); *sval; sval++)
				_putchar( *sval );
			break;

		case 'x':
			value_i = va_arg( unnamed_p, unsigned int );		
			output_int( value_i, 16 );
			break;

		case 'c':
			value_c = va_arg( unnamed_p, unsigned int );		
			_putchar( value_c );
			break;
			
		default:
			_putchar( *p );
			break;
		}
	}
	va_end( unnamed_p );

	return 0;
}

