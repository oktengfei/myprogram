#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "errno.h"
#include "string.h"
#include "unistd.h"

int main(int argc, char * argv[])
{
	int port;
	if(argc < 2)
	{
		printf("Please Input Port, Example: 8080\n");
		return 0;
	}
	else 
	{
		port = atoi(argv[1]);
		printf("0. Input port: %d \n",port);
		
		// 创建套接字
		int slisten = socket(AF_INET, SOCK_STREAM, 0);
		if(slisten == -1)
		{
			printf("socket error ! \n");
			return 0;
		}
		else
			printf("1. Create socket: %d\n",slisten);
		

		// 绑定IP端口
		struct sockaddr_in saddr_in; // 需要 "netinet/in.h"
		saddr_in.sin_family = AF_INET;
		saddr_in.sin_port = htons(port); // htons 将16位整型变量转换为网络字节顺序
		// Server ip 地址 这里选择任意地址
		saddr_in.sin_addr.s_addr = htonl(INADDR_ANY); // htonl 将32位整形~~
		// 这里saddr_in要从sockaddr_in强制转换为sockaddr类型.
		if(bind(slisten, (struct sockaddr *)&saddr_in, sizeof(saddr_in)) == -1)
		{
			printf("bind error !\n");
			return 0;
		}
		else
		{
			printf("2. bind sockaddr_in. \n");
		}

		// 开始监听
		if(listen(slisten,5) == -1)		
		{
			printf("listen error !\n");
			return 0;
		}
		else
			printf("3. start listen.\n");

		// 循环接受数据
		int accept_s;	// 连接所用的socket
		struct sockaddr_in remoteAddr;
		int Addrlen = sizeof(remoteAddr);
		char recvData[255];
		while(1)
		{
			printf("4. 等待连接....\n");
			// 获取连接的socket
			accept_s = accept(slisten,(struct sockaddr*)&remoteAddr,&Addrlen); 	
			if(accept_s == -1)
			{
				// strerror需要string.h  errno 需要 errno.h
				printf("accept error : %s\n",strerror(errno));
				return 0;
			}
			printf("4.1 Get connect .\n");
			
			while(1)
			{
				// 接受数据
				int ret = recv(accept_s, recvData, 255, 0);
				if(ret > 0)
				{
					recvData[ret] = 0;
					printf("4.2 Get data: \n%s",recvData);
				}

				// 发送数据
				scanf("%s",recvData);
				ret = strlen(recvData);
				recvData[ret]='\n';
				recvData[ret+1]=0;
				ret = send(accept_s, recvData, ret+1, 0);
				if(ret!=-1)
					printf("send success.\n");
				//close(accept_s);	// 需要unistd.h
			}
		}
		// 关闭套接字
		//close(slisten);
	}

}
