#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>	
void executeCMD(const char *cmd, char *result)   
{   
    char buf_ps[1024];   
    char ps[1024]={0};   
    FILE *ptr;   
    
	strcpy(ps, cmd);   
    
	if((ptr=popen(ps, "r"))!=NULL){    
		while(fgets(buf_ps, 1024, ptr)!=NULL){   
			strcat(result, buf_ps);   	
			if(strlen(result)>1024)   
            break;   
        }   
        
		pclose(ptr);   
        ptr = NULL;   
    }   
    else{   
        printf("popen %s error\n", ps);   
    }   
}  
	
int is_dev_on(const char *mac_addr)
{

	char cmd_buff[80] = {0};	
	char result[20] = {0};

	snprintf(cmd_buff,1024,"iw dev wlan0 station dump | grep %s | cut -c 9-25",mac_addr);

	printf("cmd_buff %s\n",cmd_buff);

	executeCMD(cmd_buff,result);

	printf("result %s\n",result);

	if(strncmp(result,mac_addr,17) == 0){
		printf("YesYesYes\n");
		return 0;
	}else{
		printf("NoNoNo\n");
		return -1;
	}
}

int main(int argc, const char *argv[])
{
	
	printf("mac:%s\n",argv[1]);
	is_dev_on(argv[1]);
	return 0;
}

