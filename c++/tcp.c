#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <net/if.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#if 0
int tcp_listen(int port)
{
    int             listenfd, ret;
    const int       on = 1;
    struct sockaddr_in server_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        return listenfd;
    }

    ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "tcp set option err, port %d error\n", port);
        close(listenfd);
        return -1;
    } 

    /* 
     * 获得原始接收缓冲区大小 
     */ 
    int rcv_size = 0; 
    socklen_t optlen;  
    optlen = sizeof(rcv_size); 
    ret = getsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen); 
    if(ret<0){ 
        printf("获取接收缓冲区大小错误\n"); 
        return -1;
    } 
     
   printf(" 接收缓冲区原始大小为: %d 字节\n",rcv_size); 

   /* 
    ** 设置接收缓冲区大小 
    **/ 
   rcv_size = 500*1024;    /* 接收缓冲区大小为8K */ 
   optlen = sizeof(rcv_size); 
   ret = setsockopt(listenfd,SOL_SOCKET,SO_RCVBUF, (char *)&rcv_size, optlen); 
   if(ret < 0){ 
       printf("设置接收缓冲区大小错误\n"); 
       return -1;
   } 

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        IMP_LOG_ERR(TAG, "tcp bind port %d error\n", port);
        close(listenfd);
        return -1;
    }

    ret = listen(listenfd, 8);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "tcp listen port %d error\n", port);
        close(listenfd);
        return -1;
    }

    return (listenfd);
}

#endif 
int send_data_to_tcp_server(const char *ip, int port, char *data, int dlen)
{
	int socket_fd;
	struct sockaddr_in socket_server_addr;
	int ret = 0;
	
	int flags;
	printf("here1\n");
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
		printf("socket_fd err\n");
        return socket_fd;
    }


	socket_server_addr.sin_family      = AF_INET;
	socket_server_addr.sin_port        = htons(port);  /* host to net, short */
 	//socket_server_addr.sin_addr.s_addr = INADDR_ANY;
	printf("here2\n");
 	if (0 == inet_aton(ip, &socket_server_addr.sin_addr))
 	{
		printf("invalid server_ip\n");
        ret = -1;
        goto exit;
	}
	printf("here3\n");
	memset(socket_server_addr.sin_zero, 0, 8);
	ret = connect(socket_fd, (const struct sockaddr *)&socket_server_addr, sizeof(struct sockaddr));	
	if (-1 == ret)
	{
		printf("connect error!\n");
        goto exit;
	}
	printf("here4\n");
    ret = send(socket_fd, data, dlen, 0);
	printf("here:%d\n",ret);
    close(socket_fd);
    if (ret <=0) {
		printf("ret 0 /-1\n");
        ret = -1;
    } else {
		printf("ret != 0\n");
        ret = 0;
    }
	
	printf("here:%d\n",ret);

exit:

    return ret;
}

#if 0
int tcp_check_socket_can_read_timeout(int sfd, int timeout_ms)
{
    fd_set r;
    int ret;
    struct timeval timeout;

    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    FD_ZERO(&r);
    FD_SET(sfd, &r);
    ret = select(sfd + 1, 0, &r, 0, &timeout);
    if (ret <= 0) {
        return ret;
    }

    if (!FD_ISSET(sfd,&r)) {
        return 0;
    }

    return 1;
}
#endif
