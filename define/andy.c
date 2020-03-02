#include <stdio.h>

#define andy_debug(fmt,arg...) 	(printf(fmt,##arg))

int main(int argc, const char *argv[])
{
	int n = 5;
	andy_debug("%d\n",n);
	return 0;
}
