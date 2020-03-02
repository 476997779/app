#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>


#define WY_DEBUG_BIND   3

#define APP_BIND_TCP_PORT   (9000)

enum {
    CMD_SSID = 0x01,
    CMD_PWD  = 0x02,
    CMD_APPID = 0x03,
    CMD_IPADDR = 0x04,
};


 int send_uuid_to_phone(const char *ip, int port)
{
    char uuid[32] = "CVUUA924Z1V4VG6GYHD1";
    //int ret = ZRT_INF_GetUID(uuid , 31);                                                                                                                                                                         
  //  if (ret < 0) {
    //    wy_error(WY_DEBUG_BIND, "ZRT_INF_GetUID() failed");
     //   return -1; 
    //}
	int ret;
    int retry = 100;
    do {
		printf("retry: %d\n",retry);
        ret = send_data_to_tcp_server(ip, port, uuid, strlen(uuid));
        if (ret < 0) {
			printf("send failed\n");
           // wy_info(WY_DEBUG_BIND, "retry = %d\n", retry);
            sleep(1);
        } else {
            break;
        }
    } while (retry--);
printf("finshed\n");
    return ret;
}



void wy_enable_eth1(void)
{
    FILE *fe = popen("/sbin/ifconfig eth1 up","r");
    sleep(1);
    pclose(fe);
}

int wy_reconnect_wlan_for_route(char *ssid,char *psk)
{
#define net_name "eth1"
	int ret;
	int fd;
	int i = 0;
	char new_ssid[20] = {0};
	char new_key[20] = {0};

	int skfd = 0;
	struct ifreq ifr;

	FILE *fh = NULL;
	FILE *fg = NULL;

	int ssid_num; 
	int key_num;
	system("/bin/rm /wds_repeater/ssid_and_key.txt");

	if((fd = open("/wds_repeater/ssid_and_key.txt", O_RDWR | O_CREAT | O_APPEND, 0666)) < 0)
	{
		printf("fail to open config\n");
		return -1;
	}


	snprintf(new_ssid,1024,"\'%s\'",ssid); 
	snprintf(new_key,1024,"\'%s\'", psk); 
	ssid_num = strlen(new_ssid);
	key_num = strlen(new_key);

	printf("new_ssid %s\n",new_ssid);	
	printf("new_key %s\n",new_key);	
	printf("ssid_num %d\n",ssid_num);	
	printf("key_num %d\n",key_num);	

	write(fd,new_ssid,ssid_num);
	write(fd,"\r\n",2);
	write(fd,new_key, key_num);

	close(fd);

	fh = popen("/bin/rm /wds_repeater/wds/wireless","r");
	usleep(300000);
	fh = popen("/bin/cp /wds_repeater/wds/wireless.bak /wds_repeater/wds/wireless","r");
	usleep(300000);
	fh = popen("echo option ssid `cat /wds_repeater/ssid_and_key.txt | sed -n '1p'` >> /wds_repeater/wds/wireless","r");
	usleep(300000);
	fh = popen("echo option key `cat /wds_repeater/ssid_and_key.txt | sed -n '2p'` >> /wds_repeater/wds/wireless","r");
	usleep(300000);

	pclose(fh);
	/**/

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd < 0){
		printf("%s:%d Open socket error!\n", __FILE__, __LINE__);
		return -1;
	}
	strcpy(ifr.ifr_name, net_name);

	if(ioctl(skfd, SIOCGIFFLAGS, &ifr) <0 ){
		printf("%s:%d IOCTL error!\n", __FILE__, __LINE__);
		printf("Maybe ethernet inferface %s is not valid!", ifr.ifr_name);
		close(skfd);
		return -1;
	}
	if(ifr.ifr_flags & IFF_RUNNING){
        fg = popen("/sbin/ifconfig eth1 down","r");
        usleep(300000);//0.3s
        pclose(fg);
        close(skfd);
	}
	else{    
		fg = popen("/bin/cp /wds_repeater/wds/wireless /etc/config","r");
		usleep(500000);
	//	system("/sbin/wifi");
		pclose(fg);
		close(skfd);
	}
	
}
