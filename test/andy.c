#include <stdio.h>

int main(int argc, const char *argv[])
{
	int a[5];
	memset(a,0,20);
	memset(a,"",1);
	printf("a = %c\n",a[0]);
	return 0;
}
