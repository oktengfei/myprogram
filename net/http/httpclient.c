#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"		// gethostbyname()
#include "arpa/inet.h"


int main(int argc, char *argv[])
{
	char* hostname;
	struct hostent* host;
	if(argc<2)
	{
		printf("Please Input hostaddr\n");
		return 0;
	}
	else
		hostname=argv[1];

	host=gethostbyname(hostname);
	if(NULL == host)
	{
		printf("host addr error!\n");
		return 0;
	}
	else
	{
		printf("host ip=%s\n",inet_ntoa(*((struct in_addr*)host->h_addr)));
		printf("host ip网络顺序:%d:%d:%d:%d\n",(unsigned char)*(host->h_addr),(unsigned char)*(host->h_addr+1),(unsigned char)*(host->h_addr+2),(unsigned char)*(host->h_addr+3));
	}


	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		printf("create sockfd error !\n");
		return 0;
	}

	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(80);
	saddr.sin_addr = *(struct in_addr*)host->h_addr;
	
	if(-1 == connect(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)))
	{
		printf("connect error.\n");
	   	return 0;
	}
	else
		printf("2. connect success .\n");

	char getbuf[]={"GET /v3/weather/now.json?key=109llqqb2lbcbc4n&location=nanyang&language=zh-Hans&unit=c HTTP/1.0\r\n"
					"User-agent:Mozilla/4.75 [en] (Win98; U)\r\n"
					"Accept:text/html\r\n"
					"Accept-language:zh-cn\r\n"
					"\r\n"};
	char recvBuf[1024];
	int res = send(sockfd,getbuf,sizeof(getbuf),0);
	if(res==sizeof(getbuf))
	{
		res=recv(sockfd,recvBuf,1024,0);
		printf("请求结果:\n%s",recvBuf);
		printf("\n");
	}
	return 0;
}
