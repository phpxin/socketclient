/**
 * 多进程模拟多用户访问服务器程序
 */

#include "apue.h"
#include "net.h"
#include <string.h>
#include "menu.h"
#include "strutil.h"

void act_login();
void act_test();
int print_menu();

int clientSockFlag = 0;
size_t pl_size = sizeof(int) ;
size_t protocol_size = sizeof(unsigned short) ;

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
	clientSockFlag = socket(AF_INET, SOCK_STREAM, 0);

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
	
	int menu = MENU_ERR;
	int run = 1;

	while(run)
	{
		menu = print_menu();
		switch(menu)
		{
			case MENU_QUIT:
				run = 0;
				break;
			case MENU_LOGIN:
				act_login();
				break;
			case MENU_TEST:
				act_test();
				break;
			default: /* MENU_ERR */
				printf("enter error please retry .");
				break;
		}
	}

	close(clientSockFlag);

	exit(EXIT_SUCCESS);
}

void act_login()
{
	int package_len = pl_size + protocol_size + 200*2;

	void *package = malloc(package_len);
	memset(package, '\0', package_len);
	
	int shift_p = 0;
	
	memcpy(package, &package_len, pl_size) ;
	shift_p += pl_size;
	
	unsigned short protocol = 1001;
	memcpy(package+shift_p, &protocol, protocol_size);
	shift_p += protocol_size;

	char account[200] = {'\0'};
	char pwd[200] = {'\0'};
	
	printf("enter account:");
	fgets(account, 199, stdin);
	su_trim(account, "\n\r");

	printf("enter password:");
	fgets(pwd, 199, stdin);
	su_trim(pwd, "\n\r");

	printf("account: %s, password: %s, login ...\n", account, pwd);
	
	memcpy(package+shift_p, account, 200);
	shift_p += 200;

	memcpy(package+shift_p, pwd, 200);

	/* send */

	send(clientSockFlag, package, package_len, 0);

	free(package);
	package = NULL;

}

void act_test()
{
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
}

int print_menu()
{
	int ret = MENU_ERR ;
	
	printf("\n\n");
	printf("menu:\n");

	printf("login : login system . \n");
	printf("quit : quit and break connection . \n");

	printf("please enter menu item : ");

	char sbuff[1024] = {'\0'} ;

	fgets(sbuff, 1023, stdin);

	if(strncmp(sbuff, "quit", 4) == 0)
	{
		ret = MENU_QUIT;
	}
	else if(strncmp(sbuff, "login", 5) == 0)
	{
		ret = MENU_LOGIN;
	}
	else if(strncmp(sbuff, "test", 4) == 0)
	{
		ret = MENU_TEST;
	}
	else
	{
		ret = MENU_ERR;
	}

	return ret;
}
