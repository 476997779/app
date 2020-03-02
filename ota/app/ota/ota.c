#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/types.h>

#define RECV_BUF_LEN (1024*100)
struct hostent *host;
char domain[256] = {0};
int port = 0;
int ret_status(char *recv_buf)
{
    if(NULL == recv_buf)
    {   
        printf("recv_buf is NULL\n");
        return -1;
    }
    int http_status = 0;
    char line[256] = {0} ;
    char *rest ;
    rest = strstr(recv_buf,"\r\n");
    if ( rest != NULL)
    {
        memcpy(line,recv_buf,rest-recv_buf);
        if(strstr(line,"200"))
        {
            http_status = 200;
        }
        else if(strstr(line,"302") || strstr(line,"301"))
        {
            http_status = 302;
        }
        else
        {
            http_status = -1;
        }
    }
    return http_status;
}
char *afresh_url(char *recv_buf)
{
    char *rest = NULL;
    char *new_url = NULL;
    char *line = NULL;
    if(NULL == recv_buf)
    {
        return NULL;
    }
    new_url = (char *)malloc(RECV_BUF_LEN);
    if(NULL == new_url)
    {
        printf("malloc new_url error!\n");
        return NULL;
    }
    if( strstr(recv_buf,"Location:") == NULL)
    {
        return NULL;
    }
    else
    {
        rest = strstr(recv_buf,"Location:")+strlen("location: ");
        line = strstr(rest,"\r\n");
        memcpy(new_url, rest,line-rest);
    }
    return new_url;
}

long ret_file_size(char *recv_buf)
{
    long file_size = 0;
    char *rest = NULL;
    char *line = NULL;
    char actual_size[RECV_BUF_LEN] = {0} ;
   
    if( NULL == recv_buf)
    {
        printf("recv %s is NULL\n",recv_buf);
        return -1;
    }
    if((strstr(recv_buf,"Content-Length")) == NULL)
    {
//        printf("Content-Length is NULL\n");
        return -1;
    }
    rest = strstr(recv_buf,"Content-Length:")+strlen("Content-Length: ");
    line = strstr(rest,"\r\n");
    memcpy(actual_size,rest,line-rest);
    file_size = atoi(actual_size);
    return file_size;
}

char *down_local_file(char *src)
{
        if(NULL == src)
        {
                return NULL;
        }
        char *line = NULL;
        char file[100] = {0};
        char *content = NULL;
        char *file_name;
        int len = 0;

        if(strstr(src,"Content-Disposition") == 0)
        {
                return NULL;
        }
        else
        {
                file_name = (char *)malloc(100);
                if(NULL == file_name)
                {
                        return NULL;
                }
                line = strstr(src,"filename=") + 9;
                if( NULL != line)
                {
                        content = strstr(line,"\r\n");
                        if( content != NULL)
                        {
                                len =  strlen(line) - strlen(content);
                                memcpy(file_name,line,len);
                                line[len] = 0;
                        }
                }
        }
        return file_name;
}

char *down_file(char *src, char c)
{
    if(NULL == src)
    {
        printf("The src %s is null\n",src);
        exit(-1);
    }
    int len = 0;
    len = strlen(src);
    while(src[len-1])
    {
        if( strchr(src + (len - 1),c))
        {
            return (src + (len - 1));
        }
        else
        {
            len--;
        }
    }
    return NULL;
}

char *http_request(char *url)
{
    char *url_dress = NULL;
    char *file_dress = NULL;
    char *ret = NULL;
    char *request;
    struct hostent *host;
   
    url_dress = url+strlen("http://");
    file_dress = strchr(url_dress,'/');
    if (file_dress)
    {
        memcpy(domain,url_dress,file_dress-url_dress);
    }
    ret = strstr(domain,":");
    if(ret)
    {
        port = atoi(ret);
    }
    else
    {
        port = 80;
    }
    request = (char *)malloc(RECV_BUF_LEN);
    if(NULL == request)
    {
        printf("malloc request error!\n");
        exit(-1);
    }
    sprintf(request,"GET %s HTTP/1.1\r\nHost:%s\r\n\
User-Agent:Mozilla/5.0 (Windows NT 6.1; rv:12.0) Gecko/20100101 Firefox/12.0\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3\r\n\
Accept-Encoding: gzip,deflate\r\n\
Connection: keep-alive\r\n\r\n",file_dress,domain);
    return request;
}
int wy_ota_download_file(char *URL, char *fname)
//int main(int argc, char *argv[])
{
    char *url;
    int sockefd;
    char *ret = NULL;
    char *buffer ;
    char *head_buffer = NULL;
    char header[RECV_BUF_LEN] = {0};
    struct sockaddr_in server_addr;
    FILE *fp = NULL;
    char *request;
    int i = 0;
    char *file_dress = NULL;
    char *file_name;
    char *local_file = NULL;
    int send = 0;
    int totalsend = 0;
    int nbytes = 0;
    char *url_address = NULL;
    int http_status = 0;
    struct timeval start_time,end_time;
    float timeuse;
    FILE *file ;
    char url_link[200] = {0};
    char *url_ret;
    /*
	if (argc != 2)
    {
        fprintf(stderr,"Usage: %s web address \n",argv[0]);
        exit(-1);
    }
	*/
    //url = argv[1];
    url = URL;
	do {
        request = http_request(url);
        if((host = gethostbyname(domain)) == NULL)
        {
            printf("Get host name error!\n");
            exit(-1);
        }
        gettimeofday(&start_time,NULL);
        if((sockefd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        {
            printf("Socket error!\n");
            exit(-1);
        }
        bzero(&server_addr,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);

        if (connect(sockefd, (struct sockaddr *)(&server_addr),sizeof(struct sockaddr)) == -1)
        {
            printf("connect error!\n");
            exit(-1);
        }
        send = 0;
        totalsend = 0;
        nbytes = strlen(request);
        while(totalsend < nbytes)
        {
            send = write(sockefd ,request + totalsend,nbytes - totalsend);
            if (send == -1)
            {
                printf("send error!\n");
                exit(-1);
            }
            totalsend += nbytes;
            printf("%d bytes send OK!\n",totalsend);
        }

        long file_size = 0;
        int mark = 1;
        int count=0;
        buffer = (char *)malloc(RECV_BUF_LEN);
        do   
        {
            memset(buffer,0,RECV_BUF_LEN);
            nbytes = recv(sockefd,buffer,RECV_BUF_LEN,0);

            if (mark > 0) {
                mark = 0;
                head_buffer = strstr(buffer,"\r\n\r\n")+4;
                memcpy(header,buffer,head_buffer-buffer);
                printf("header: \n %s\n",header);
                file_size = ret_file_size(header);
                http_status = ret_status(header);
                if(http_status == 200)
                {
                    file_name = down_local_file(header);
					if(NULL == file_name)
                    {
                        file_name = down_file(url,'/')+1;
						file_name = fname;
						fp = fopen(file_name,"w");
                        if(NULL == fp)
                        {
                            printf("open file %s failed!",local_file);
                            exit(-1);
                        }
                    }
                    else
                    {
						file_name = fname;
						fp = fopen(file_name,"w");
                        if(NULL == fp)
                        {
                            printf("open file %s failed!",local_file);
                            exit(-1);
                        }
                    }
                    count = fwrite(head_buffer,1,nbytes-(head_buffer - buffer),fp);
        //        printf("%s\t%d",head_buffer,strlen(head_buffer));
                //printf("head_buffer   %s\n",head_buffer);
                //printf("--------------\n");
                    url = NULL;
                }
                else if(http_status == 302)
                {
                    url = afresh_url(header);
                }
            }
            else
            {
                fwrite(buffer,1,nbytes,fp);
                fflush(fp);
                count+=nbytes;
            }
            if(count == file_size)
            {
                break;
            }
        }while(nbytes > 0);
        printf("total:%d\n",count);
        free(request);
        free(buffer);
        close(sockefd);
    }while(url != NULL );
    gettimeofday(&end_time,NULL);
    timeuse = 1000000*(end_time.tv_sec - start_time.tv_sec) + end_time.tv_usec - start_time.tv_usec;
    timeuse /= 1000000;
    printf("Used time: %f\n",timeuse);
    fclose(fp);
    free(url);
    return 0;
}

int wy_ota_check_md5(void)
{
	int ret = 5;
	ret = system("md5sum -c ./md5.txt");
	printf("ret = %d \n",ret);
	
	if(0 == WEXITSTATUS(ret)){
		printf("md5 check success\n");
		return 0;
	}else{
		printf("md5 check failed\n");
		return -1;
	}
}

int wy_ota_update(void)
{
	int ret = 6;
	ret  = system("./update.sh");
	printf("ret = %d \n",ret);
	
	if(0 == WEXITSTATUS(ret)){
		printf("update success\n");
		return 0;
	}else{
		printf("update failed\n");
		return -1;
	}
}

int wy_unzip_file(const char *zip,const char *dir)
{
	int ret = 50;
	char buff[64] = {0};
	
	sprintf(buff,"unzip %s -d %s",zip,dir);
	
	printf("buff = %s \n",buff);
	
	ret = system(buff);
	
	printf("ret = %d\n",ret);
	
	if(0 == WEXITSTATUS(ret)){
		printf("unzip success\n");
		return 0;
	}else{
		printf("unzip failed\n");
		return -1;
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
    ret = wy_ota_download_file("http://wanyee-doorbell.oss-cn-beijing.aliyuncs.com/update_station.zip","update.zip");
	if(ret != 0){
		printf("download failed \n");
	return -1;
	}
	
	ret = wy_unzip_file("~/app/last/update.zip","~/app/last/");
	if(ret != 0){
		printf("unzip failed \n");
	return -1;
	}
	
	ret = wy_ota_check_md5();
	if(ret != 0){
		printf("md5 check failed \n");
	return -1;
	}

	ret = wy_ota_update();
	if(ret != 0){
		printf("update failed \n");
	return -1;
	}
	
	printf("ota success\n");
	return 0;
}
