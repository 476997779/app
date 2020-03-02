#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, const char *argv[])
{
	char str[12] ;

	int i = 0;

	memset(str,49,12);
	memset(str+1,0,12);
	
	strcpy(&str[3],"he");
	
	printf("str = %s \n", str);
	
	return 0;
}
