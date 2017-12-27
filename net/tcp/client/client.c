#include "stdio.h"
#include "sys/socket.h"		// socket 函数
#include "errno.h"			// errno
#include "string.h"			// strerror
#include "netinet/in.h"		// sockaddr_in
#include "stdlib.h"			// atoi函数
//#include "netinet/ip.h"	
#include "arpa/inet.h"		// inet_addr 函数

int main(int argc, char* argv[])
{
	// 获取ip地址和端口
	int port;
	char *ip_str;
	int res;
	if(argc < 3)
	{
		printf("Please Input IP & Port,Example: 127.0.0.1 8080.\n");
		return 0;
	}
	else
	{
		port = atoi(argv[2]);
		ip_str = argv[1];
		printf("0. Set Server IP and Port: %s:%d\n",ip_str,port);
	}

	// 创建套接字
	int sclient = socket(AF_INET, SOCK_STREAM,0);
	if(sclient == -1)
	{
		printf("socket create error :%s\n",strerror(errno));
		return 0;
	}
	else
		printf("1. create sclient socket.\n");
	
	struct sockaddr_in client_sockaddr;		// 定义IP地址结构
	client_sockaddr.sin_port = htons(port);	// 指定端口号
	client_sockaddr.sin_family = AF_INET;	// 指定结构类型为TCP/IP
	client_sockaddr.sin_addr.s_addr = inet_addr(ip_str);	// 将要连接的IP地址转换为int型
	if(-1 == connect(sclient, (struct sockaddr*)&client_sockaddr, sizeof(client_sockaddr)))
	{
		printf("connect error:%s\n",strerror(errno));
		return 0;
	}
	else
		printf("2. connect success .\n");

	char buf[255];
	while(1)
	{
		scanf("%s",buf);
		res = strlen(buf);
		buf[res] = '\n';	buf[res+1]=0;
		res = send(sclient, buf, res+1,0);
		if(res != -1)	printf("send success.\n");
		res = recv(sclient, buf, 255, 0);
		if(res > 0)
		{
			buf[res]=0;
			printf("%s",buf);
		}
	}
}


