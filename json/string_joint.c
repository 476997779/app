#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
struct wy_video_pack_par {
	uint64_t time;
	char *suid;
	char *uid;
	int type;
	char *title;
	char *data;
	int dlen;
};

char * wy_format_package_video(struct wy_video_pack_par *inpar, uint32_t *olen)
{
	int len = 0;
	int malloc_size = 0;
	

	char *msg = NULL;

	len = snprintf(msg,0,"DEV:%s:msg:{\"time\":\"%lld\",\"suid\":\"%s\",\"uid\":\"%s\",\"reserved\":\"video\",\"type\":\"%d\",\"title\":\"%s\",\"dlen\":\"%d\"}--DATA:",  \
			inpar->uid,inpar->time,inpar->suid,inpar->uid,inpar->type,inpar->title,inpar->dlen);

	printf("dlen = %d\n",len);
	/*+1 for '\0'*/	
	malloc_size = len + inpar->dlen + 1; 
	printf("malloc_size = %d\n",malloc_size);

	msg = (char *)malloc(malloc_size);
	if(NULL == msg){
		printf("malloc msg failed\n");
		*olen = 0;
		return NULL;
	}	
	len = snprintf(msg,malloc_size,"DEV:%s:msg:{\"time\":\"%lld\",\"suid\":\"%s\",\"uid\":\"%s\",\"reserved\":\"video\",\"type\":\"%d\",\"title\":\"%s\",\"dlen\":\"%d\"}--DATA:",  \
			inpar->uid,inpar->time,inpar->suid,inpar->uid,inpar->type,inpar->title,inpar->dlen);

	memcpy(msg+len,inpar->data,inpar->dlen);	
	*olen = len + inpar->dlen;
	
	return msg;
}


int main(int argc,  char *argv[])
{
	char *buff= NULL;
	struct wy_video_pack_par test1 = {1255565656562,"12345678","ecW9fW0dW9fWf9W2c",1,"Motion-2019-10-29.mp4",buff,6};
	
	uint32_t olen = 0;
	char * msg = NULL;
	
	int fd;
	int ret;
	struct stat stat_buf;

	test1.title = argv[1];	

	ret = stat(argv[1],&stat_buf);
	if(-1 == ret){
		perror("fail to stat");
		exit(1);
	}		
	
	if((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("fail to open");
		return -1;
	}
	
	test1.dlen = stat_buf.st_size;
		
	buff = (char * )malloc(test1.dlen);
	if(NULL == buff){
		printf("malloc buff failed\n");
		return 0;
	}	
	
	printf("dlen = %d\n",test1.dlen);

	ret = read(fd, buff, test1.dlen);

	printf("read sieze = %d\n",ret);
	
	test1.data = buff;
	
	msg = wy_format_package_video(&test1,&olen);
	
#if  1 /*debug*/
	printf("total=  %d\n",olen);
	printf("%s\n",msg);
	printf("%#x\n",msg[55555]);
	printf("%#x\n",msg[33268]);
#endif 
	
	free(msg);
	free(buff);
	buff = NULL;
	msg = NULL;
	return 0;

}
