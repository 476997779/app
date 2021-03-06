#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/input.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATH_INPUT "/dev/input"
#define PATH_SHELL "/bin/test.sh"
#define PATH_TEXT  "/tmp/hostapd.conf"
#define PATH_STA_CFG   "/config/sample_uid"


static int g_ev_fd;
static bool button_flag = false;
char psw[20] = {0};	
int SU_Key_OpenEvent(void)
{
	DIR *dir;
	struct dirent *de;
	int ret;
	char path[BUFSIZ];

	char name[80];
	dir = opendir(PATH_INPUT);
	if (dir == NULL) {
		fprintf(stderr, "open input dir error");
		return -1;
	}
	while ((de = readdir(dir)) != NULL) {

		int fd = openat(dirfd(dir), de->d_name, O_RDONLY);
		if (fd < 0) {
			printf("fd <0\n");
			fprintf(stderr, "openat error");
			continue;
		}	
		printf("de->name: %s\n",de->d_name);
	
		ret = ioctl(fd, EVIOCGNAME(sizeof(name) - 1), &name);
		close(fd);

		printf("name: %s\n",name);
		if (ret < 1) {
			if (strcmp(de->d_name, ".") && strcmp(de->d_name, ".."))
				fprintf(stderr, "could not get device name for %s/%s: %s",
						   PATH_INPUT, de->d_name, strerror(errno));
			continue;
		}

		printf("SSS---de->name: %s\n",de->d_name);
		if (!strcmp(name, "10000000.palmbus:gpio-keys")) {
			printf("here\n");
			sprintf(path, "%s/%s", PATH_INPUT, de->d_name);
			fprintf(stderr, "GPIO key device : %s", path);
			break;
		}
	}
	closedir(dir);
	printf("path:%s\n",path);
	int evfd = open(path, O_RDONLY);
	if (evfd < 0) {
		fprintf(stderr, "open %s error: %s\n", path, strerror(errno));
		return -1;
	}
	printf("name  = %s\n",name);
	return evfd;
}

int SU_Key_ReadEvent(int evfd, int *keyCode, bool *is_pressed)
{
	int ret;
	struct input_event in_event;

	while (1) {
		ret = read(evfd, &in_event, sizeof(struct input_event));
		if (ret <= 0) {
			printf("read key event error\n");
			//ZRTLOG_ERR("read key event error: %s", strerror(errno));
			return -1;
		}

		if (in_event.type != EV_KEY)
			continue;

		//ZRTLOG_DBG("Key:%d %s ", in_event.code & 0xff, in_event.value ? "pressed" : "released");

		if (in_event.value) {
			*is_pressed = true;
		} else {
			*is_pressed = false;
		}
		*keyCode = in_event.code & 0xff;
		
		break;
	}

	return 0;
}

static void get_pwd_by_sn(char *ssid,  char*psw)                                                                                                                                                              
{
    int i;
    for (i= 0; i< 64; i++) {
        if (ssid[i] == 0x0)
            break;
        
        if (ssid[i] >= '0' && ssid[i] <='9')
            psw[i] = '0' + 9 - (ssid[i] - '0');
        else if (ssid[i] >= 'a' && ssid[i] <= 'z')
            psw[i] = 'A' + 25 - (ssid[i] - 'a');
        else if (ssid[i] >= 'A' && ssid[i] <= 'Z')
            psw[i] = 'a' + 25 - (ssid[i] - 'A');
    }
}

void get_wifi_ssid_tail(char *buff)
{
	int ret;
	int fd;
	char tmp[25] = {0};
	int i = 0;
	FILE *fp=NULL;
#if 0
	ret = system(PATH_SHELL);
	if(-1 == ret){
		printf("system shell failed\n");
		return -1;
	}
#endif 
	
	if((fd = open(PATH_STA_CFG, O_RDWR | O_CREAT | O_APPEND, 0666)) < 0)
	{
		printf("fail to open config\n");
		return;
	}
	printf("fd=%d\n",fd);
	ret = read(fd,buff,24);
	printf("buff=%s\n",buff);
	if(0 == ret){
		printf("222222no date to read \n");
		return;
	}
	
	for(i = 0;i < 8;i++){
//	printf("buff:%c\n",buff[i]);
		tmp[0+i] = buff[10+i];
	}
	printf("tmp %s\n",tmp);
	
	/*
	if((fd = open(PATH_TEXT, O_RDWR | O_CREAT | O_APPEND,  0666)) < 0)
	{		printf("fail to open\n");
		return -1;
	}
	*/
	fp=fopen(PATH_TEXT,"ar");
	

	//write(fd,tmp,8);
	
	char s[50] = {0}; 
	char d[50] = {0}; 
	sprintf(s, "ssid=Zeratul_%s\n",tmp);
	printf("%s\n",s);

	printf("%p\n",fp);
	fprintf(fp,"%s",s);
	
	get_pwd_by_sn(tmp,psw);	
	
	sprintf(d, "wpa_passphrase=%s\n",psw);
	printf("%s\n",d);
	fprintf(fp,"%s",d);
	fclose(fp);
	//write(fd,psw, 8);
}


int main(int argc, const char *argv[])
{
	int evfd;
	int keyCode;
	bool is_pressed;
	char buff[25] = {0};
	int fd = 0;
#if 0	
	evfd = SU_Key_OpenEvent();
	
	SU_Key_ReadEvent(evfd,&keyCode,&is_pressed);
	
	printf("keyCode = %x\n"
			"is_pressed %d\n",keyCode,is_pressed);
	
#endif
	
	
	get_wifi_ssid_tail(buff);
	

//	printf("psw: %s\n",psw);
//	printf("psw: %s\n",psw);

	usleep(300000);

#if 0
#ifndef APP_START
#define APP_START
	fp = popen("/lib/zrt_sta_daemon --conf_path=/config/station.conf &","r");
	sleep(5);
	fh = popen("/lib/zrt_sta &","r");
	sleep(5);
	pclose(fp);
	pclose(fh);
#endif
#endif 
	return 0;
}


