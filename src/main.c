/**
 * chat 客户端
 */

#include "apue.h"
#include "net.h"
#include <string.h>
#include "menu.h"
#include "strutil.h"
#include "action.h"

static int print_menu();

int clientSockFlag = 0;

int main(int argc, char *argv[])
{
	if(argc<3){
		printf("usage chatclient {ip} {port} \n");
		exit(EXIT_FAILURE);
	}

	const char *ip = argv[1];
	const int port = atoi(argv[2]);

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

static int print_menu()
{
	int ret = MENU_ERR ;
	
	printf("\n\n");
	printf("menu:\n");

	printf("\tlogin : login system . \n");
	printf("\ttest : run test . \n");
	printf("\tquit : quit and break connection . \n\n");

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
