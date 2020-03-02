#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern int wy_reconnect_wlan_for_route(char *ssid,char *psk);
extern  int send_uuid_to_phone(const char *ip, int port);
extern void wy_enable_eth1(void);


int main(int argc, const char *argv[])
{
	//wy_reconnect_wlan_for_route("HUAWEI-2162","xuwanhao");
	wy_reconnect_wlan_for_route("wanyee","#wanyee*123");
	sleep(2);
	send_uuid_to_phone("192.168.3.41",9000);
	system("/sbin/ifconfig eth1 up");
	return 0;
}
