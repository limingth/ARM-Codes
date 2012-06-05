
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

	while((*cp++ = *src++))
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

/*
 * state: 
 * 0: start
 * 1: O, OS
 * 2: OS' '
 * 3: OS' '=
 * 4: OS' '=' '
 * 5: OS' '=' 'bootloader
 */

int isdigit(int c)
{
	return c >= '0' && c <= '9';
}

int isalpha(int c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isname(char c)
{
	return isdigit(c) || isalpha(c) || (c == '.');
}

char * get_key_value(const char * key, char * buf, char * value)
{
	int state = 0;
	int i = 0;
	char name[16];
	int n = 0;
	int v = 0;

	while (buf[i] != '\0')
	{
		char c = buf[i];
	//	printf("c = %c\n", c);

		if ((state == 0 || state == 1) && isalpha(c))
		{
			name[n++] = c;
			state = 1;
		}
		else
		if (state == 1 && c == ' ')
		{
			name[n] = '\0';
			state = 2;
		}
		else
		if ((state == 1 || state == 2) && c == '=')
		{
			name[n] = '\0';
			state = 3;
		}
		else
		if (state == 3 && c == ' ')
		{
			state = 4;
		}
		else
		if ((state == 3 || state == 4) && isname(c))
		{
			value[v++] = c;
			state = 5;
		}
		else
		if (state == 5 && isname(c))
		{
			value[v++] = c;
			state = 5;
		}
		else
		if (state == 5 && c == '\n')
		{
			value[v] = '\0';
			state = 0;

			//printf("<%s> : <%s> \n", name, value);

			// clear to new begin
			n = 0;
			v = 0;
			if (strcmp(name, key) == 0)
				return value;
		}
	
		i++;
	}

	return value;
}



