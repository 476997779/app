#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <pthread.h>
#include <dirent.h>
#include <ctype.h>
#define READ_BUF_SIZE 1024
int executeCMD(const char *cmd, char *result)   
{   
    char buf_ps[1024];   
    char ps[1024]={0};   
    FILE *ptr;   
    char *str = NULL;
    char *str1 = NULL;
	strcpy(ps, cmd);   
    
	if((ptr=popen(ps, "r"))!=NULL){ 
		sleep(2);
		while(fgets(buf_ps, 1024, ptr)!=NULL){   
			strcat(result, buf_ps);   	
			if(strlen(result)>1024)   
            break;   
        }   
		pclose(ptr);   
        ptr = NULL;   
       	str = strstr(result, cmd);
       	if ((str1 = strstr(str, cmd)) == NULL) {
			return -1;
		} else {
			printf("::%s\n", str);
			printf("hello\n");
			return 0;
		}
    }   
    else {   
		printf("popen %s error\n", ps);   
        return -1;
    }   
} 
//存在返回1，不存在返回0
int judge_pid_exist(char* pidName)
{
	DIR *dir;
	struct dirent *next;
	int i = 0;

	FILE *status;
	char buffer[READ_BUF_SIZE];
	char name[READ_BUF_SIZE];

	///proc中包括当前的进程信息,读取该目录
	dir = opendir("/proc");
	if (!dir)
	{
		printf("Cannot open /proc\n");
		return 0;
	}

	//遍历
	while ((next = readdir(dir)) != NULL) 
	{
		//跳过"."和".."两个文件名
		if ((strcmp(next->d_name, "..") == 0) || (strcmp(next->d_name, ".") == 0))
		{
			continue;
		}

		//如果文件名不是数字则跳过
		if (!isdigit(*next->d_name))
		{
			continue;
		}

		//判断是否能打开状态文件
		sprintf(buffer,"/proc/%s/status",next->d_name);
		if (!(status = fopen(buffer,"r"))) 
		{
			continue;
		}

		//读取状态文件
		if (fgets(buffer,READ_BUF_SIZE,status) == NULL) 
		{
			fclose(status);
			continue;
		}
		fclose(status);

		//读取PID对应的程序名，格式为Name:	程序名
		sscanf(buffer,"%*s %s",name);
		//判断程序名是否符合预期

		if (strcmp(name,pidName) == 0) 
		{
			//符合
			closedir(dir);

			return 1;
		}
	}

	closedir(dir);

	return 0;
}


int wy825_open_ap()
{
	char result[2048];
	int ret;
	system("ifconfig wlan0 192.168.200.1 netmask 255.255.255.0 up");
	sleep(1);
	system("killall -9 hostapd");
	sleep(1);
	system("hostapd -B -dd /etc/hostapd.conf");
	sleep(3);
	ret =judge_pid_exist("hostapd");
	if (!ret) {
		printf("error\n");
		printf("error\n");
		printf("error\n");
		printf("error\n");
		return -1;
	} else {
		printf("success\n");
		printf("success\n");
		printf("success\n");
		return 0;
	}

}

int wy825_close_ap()
{
	//system("ifconfig wlan0 192.168.200.1 netmask 255.255.255.0 down");
	sleep(1);
	system("killall -9 hostapd");
}

int main(int argc, const char* argv[]) 
{
	if (atoi(argv[1]) == 1) {	
		wy825_open_ap();	
	} else if (atoi(argv[1]) == 2) {
		wy825_close_ap();
	}
	return 0;
}
