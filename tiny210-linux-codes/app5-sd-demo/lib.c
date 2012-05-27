
#include "uart.h"

void delay(void)
{
	int i;
	
	for (i = 0; i < 0x100000; i++)
		;
}

int atoi(char * buf)
{
	int value = 0;	
	int base = 10;
	int i = 0;
	
	if (buf[0] == '0' && buf[1] == 'x')
	{
		base = 16;
		i = 2;
	}
	
	// 123 = (1 * 10 + 2) * 10 + 3
	// 0x1F = 1 * 16 + F(15)	
	while (buf[i])
	{
		int tmp;
		
		if (buf[i] <= '9' && buf[i] >= '0') 
			tmp = buf[i] - '0';
		else
			tmp = buf[i] - 'a' + 10;
					
		value = value * base + tmp;
		
		i++;
	}
	
	return value;
}

int strcmp(const char * s1, const char * s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return 0;	
		s1++;
		s2++;				
	}
	
	return *s1 - *s2;
}

int strncmp ( char * s1, char * s2, int n)
{
	if ( !n )
		return(0);

	while (--n && *s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	return (*s1 - *s2);
}

char* strcpy(char * dst, const char * src)
{
	char * cp = dst;

	while( *cp++ = *src++ )
		;

	return( dst );
}

char * strncpy ( char * dst, const char * src, int count )
{
	char *start = dst;

	while (count && (*dst++ = *src++))
		count--;

	if (count)
		while (--count)
			*dst++ = '\0';

	return(start);
}

void *memcpy(void *dest, const void *src, int count)
{
	// assert( (dest!=NULL)&&(src!=NULL) );

	 char *tmp_dest = (char*)dest;
	 char *tmp_src = (char*)src;

	 while( count--)//不对是否存在重叠区域进行判断
		  *tmp_dest++ = *tmp_src++;

	 return dest;
}
