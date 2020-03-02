#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	FILE *fh = NULL;
	int i = 0;
	
	for(i = 0; i < 100; i++){
	
	printf("i = %d\n",i);

	fh = popen("/bin/rm /wds_repeater/wds/wireless","r");
	usleep(300000);
	
	fh = popen("/bin/cp /wds_repeater/wds/wireless.bak /wds_repeater/wds/wireless","r");
	usleep(300000);
	
	fh = popen("echo option ssid `cat /wds_repeater/ssid_and_key.txt | sed -n '1p'` >> /wds_repeater/wds/wireless","r");
	usleep(300000);
	
	fh = popen("echo option key `cat /wds_repeater/ssid_and_key.txt | sed -n '2p'` >> /wds_repeater/wds/wireless","r");
	usleep(300000);

	pclose(fh);
	}
	return 0;
}
