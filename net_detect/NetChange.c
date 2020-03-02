#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <pthread.h>

#define NET_DEVICE "eth1"

#define ETH 0
#define WIFI 1

static int old_status;
static int new_status;

void reconfig_net(void)
{

	
	FILE *fd = NULL;
	FILE *fh = NULL;
	old_status = new_status;
	
	if(new_status == ETH){
	fd = popen("/bin/cp /wds_repeater/ap/*  /etc/config/","r");
	fh = popen("/sbin/wifi reload","r");
	}else{
	fd = popen("/bin/cp /wds_repeater/wds/*  /etc/config/","r");
	fh = popen("/sbin/wifi reload","r");
	}
	pclose(fd);
	pclose(fh);
	system("/sbin/wifi reload");
}
static int net_detect(char* net_name)
{
    int skfd = 0;
	struct ifreq ifr;
	skfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(skfd < 0)
    {
        printf("%s:%d Open socket error!\n", __FILE__, __LINE__);
        return -1;
    }

	strcpy(ifr.ifr_name, net_name);
    
	if(ioctl(skfd, SIOCGIFFLAGS, &ifr) <0 )
    {
        printf("%s:%d IOCTL error!\n", __FILE__, __LINE__);
        printf("Maybe ethernet inferface %s is not valid!", ifr.ifr_name);
        close(skfd);
        return -1;
    }

    if(ifr.ifr_flags & IFF_RUNNING)
    {
        close(skfd);
        //printf("~~~~~~~~~~eth1 running\n");
		new_status = ETH;
    }
    else
    {    
		close(skfd);
		//printf("~~~~~~~~~~NO-running\n");
    	new_status = WIFI;
	}
	if(new_status != old_status){
		reconfig_net();
	}
}


int main(int argc, const char *argv[])
{
	old_status = ETH;
	new_status = ETH;
	
	while(1){	
		net_detect(NET_DEVICE);	
		sleep(1);
	}
	return 0;
}
