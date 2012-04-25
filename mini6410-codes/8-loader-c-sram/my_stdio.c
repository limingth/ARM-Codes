#include "uart0.h"
#include "my_stdio.h"

static int char_num = 0;

char* my_gets( char *s)
{
	char *t = s;
	char ch;
	int temp;
	while((ch = uart0_getchar()) != '\r')
	{
		temp = my_putchar(ch);
		if(ch == '\b')
		{
			if(temp == 0) s--;
		}
		else *s++ = ch; 
	}

	*s = '\0';
	return t;
}

void my_puts(const char *s)
{
	while(*s != '\0')
	{
		uart0_putchar(*s++);
	}
}

int my_strlen(const char *s)
{	
	int i =0;
	while(*s++ != '\0') i++;
	return i;
}

int my_strcmp(const char *s1,const char *s2)
{
	if(s1 == NULL || s2==NULL) return -1;
	for(;*s1 == *s2 && *s1!='\0' && *s2!='\0';s1++,s2++);
	if(*s1 == '\0' && *s2=='\0') return 0;
	return *s1 - *s2;
}

int my_strncmp(const char *s1,const char *s2,int len)
{
	
	int i=0;
	if(s1 == NULL || s2==NULL) return -1;
	for(i = 0;*s1 == *s2 && *s1!='\0' && *s2!='\0' && i<len;s1++,s2++,i++);
	if(i == len) return 0;
	return *s1 - *s2;
}

int my_strcpy(char *dst, const char *src)
{
	if(dst ==NULL || src==NULL) return -1;
	while ((*dst++ = *src++) != '\0');

	return 0;
}

unsigned int atox(char *p)
{
	unsigned int x = 0;
	for(;*p != '\0';p++)
	{
	switch(*p)
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			x <<= 4; 
			x += (int)(*p - '0');
			break;
		case 'A':
		case 'a':
			x = x<<4 + 10;
			break;
		case 'B':
		case 'b':
			x = x<<4 + 11;
			break;
		case 'C':
		case 'c':
			x = x<<4 + 12;
			break;
		case 'D':
		case 'd':
			x = x<<4 + 13;
			break;
		case 'E':
		case 'e':
			x = x<<4 + 14;
			break;
		case 'F':
		case 'f':
			x = x<<4 + 15;
			break;
		default:
			return -1;
	}
	}
	return x;
}

int my_putchar(char ch)
{	
	/* read tx ready flag, when =1 break */
	
	if(ch == '\b') 
	{
	  if(char_num>0)
	   {
		uart0_putchar('\b');
		uart0_putchar(' ');
		uart0_putchar('\b');
		char_num--;
		return 0;
	   }
	  else return -1;
	}
	else if(ch == '\r') 
	{
		uart0_putchar('\r');
		uart0_putchar('\n');
		char_num = 0;
		return 0;
	}
	else
	{
		uart0_putchar(ch);
		char_num++;
		return 0;
	}
}

char my_getchar(void)
{
	char ch;
	ch = uart0_getchar();
	return ch;
}
