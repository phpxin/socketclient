/**
 * 多进程模拟多用户访问服务器程序
 */

#include "apue.h"
#include "net.h"
#include <string.h>

int main(int argc, char *argv[])
{
	if(argc<3){
		printf("usage chatclient {ip} {port} \n");
		exit(EXIT_FAILURE);
	}

	const char *ip = argv[1];
	const int port = atoi(argv[2]);

	printf("child process is running ...\n");
	printf("enter 'quit' stop \n");
	int clientSockFlag = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	inet_pton(AF_INET, ip, &serverAddr.sin_addr.s_addr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	socklen_t serverSockLen = sizeof(serverAddr);

	int flag = connect(clientSockFlag, (struct sockaddr *)&serverAddr, serverSockLen);

	if(flag<0){
		perror("connect");
		exit(EXIT_FAILURE);
	}

	int package_len = 0; /* 数据包大小 */
	size_t pl_size = sizeof(int) ;
	void *package = NULL;

	while(1)
	{
		char sbuff[1024] = {'\0'} ;

		fgets(sbuff, 1023, stdin);

		if(strncmp(sbuff, "quit", 4) == 0)
		{
			break;
		}
		
		int _l = strlen(sbuff) + 1; /* +\0 */

		package_len = pl_size + _l ;

		package = malloc(package_len);

		memcpy(package, &package_len, pl_size);
		memcpy(package+pl_size, sbuff, _l);

		send(clientSockFlag, package, package_len, 0);

		free(package);
		package = NULL;
	}
	
	close(clientSockFlag);

	exit(EXIT_SUCCESS);
}
