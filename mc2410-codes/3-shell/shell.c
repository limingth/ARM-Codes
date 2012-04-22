
void shell_parse(char * buf, int * argc, char * argv[])
{
	// buf[] = "mw      0x1000     0x0"
	int status = 0;
	char c;	
	int i = 0;
	
	*argc = 0;	
	while ((c = buf[i]) != '\0')
	{
		if ( c != ' ' && status == 0 )
		{
			status = 1;
			argv[(*argc)++] = buf + i;			
		}
		
		if ( c == ' ' && status == 1 )
		{
			status = 0;
			buf[i] = '\0';		
		}
			
		i++;			
	}	
	
	return;	
}
