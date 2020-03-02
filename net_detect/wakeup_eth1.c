#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	FILE *fe = NULL;

	fe = popen("/sbin/ifconfig eth1 up","r");
	
	sleep(1);

	pclose(fe);
	return 0;
}
