/**
 * chat 客户端
 */

#include "apue.h"
#include "net.h"
#include <string.h>
#include "menu.h"
#include "strutil.h"
#include "action.h"
#include "pthread.h"
#include "sys/epoll.h"

#define MAX_EFDS 1	/* only test client readable */

static int print_menu();
static void *thread_func(void *udata);

int ep_fd = -1 ;
struct epoll_event *ep_ok = NULL;

int clientSockFlag = 0;

struct user my;

pthread_t thread_c1;

int main(int argc, char *argv[])
{
	if(argc<3){
		printf("usage chatclient {ip} {port} \n");
		exit(EXIT_FAILURE);
	}
	
	int flag = pthread_create(&thread_c1, NULL, thread_func, NULL);
	if(flag != 0)
	{
		printf("run child thread failed error num is %d", errno);
		exit( -1 );
	}

	my.id = 0;	/* init user */

	const char *ip = argv[1];
	const int port = atoi(argv[2]);

	clientSockFlag = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	inet_pton(AF_INET, ip, &serverAddr.sin_addr.s_addr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	socklen_t serverSockLen = sizeof(serverAddr);

	flag = connect(clientSockFlag, (struct sockaddr *)&serverAddr, serverSockLen);

	if(flag<0){
		perror("connect");
		exit(EXIT_FAILURE);
	}

	ep_fd = epoll_create(5);
	
	struct epoll_event _event;
	_event.data.fd = clientSockFlag;
    _event.events = EPOLLIN ;

	flag = epoll_ctl(ep_fd, EPOLL_CTL_ADD, clientSockFlag, &_event);
    if (flag == -1) {
		printf("epoll_ctl failed error num is %d", errno);
		close(clientSockFlag);
		close(ep_fd);
        exit( -1 );
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
			case MENU_MSG:
				act_msg();
				break;
			default: /* MENU_ERR */
				printf("enter error please retry .");
				break;
		}
	}

	close(clientSockFlag);
	close(ep_fd);

	exit(EXIT_SUCCESS);
}

static void *thread_func(void *udata)
{
	ep_ok = (struct epoll_event *)calloc( MAX_EFDS, sizeof(struct epoll_event));
	int i = 0;
	while(1)
	{
		int isok = epoll_wait(ep_fd, ep_ok, 1, -1) ;
		for( i=0; i<isok; i++)
		{
			if(ep_ok[i].events & EPOLLIN)
			{
				recv_msg();
			}
		}
	}


	pthread_exit( NULL );
}

static int print_menu()
{
	int ret = MENU_ERR ;
	
	printf("\n\n");
	printf("menu:\n");

	printf("\tlogin : login system . \n");
	printf("\ttest : run test . \n");
	printf("\tmsg : send message . \n");
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
	else if(strncmp(sbuff, "msg", 3) == 0)
	{
		ret = MENU_MSG;
	}
	else
	{
		ret = MENU_ERR;
	}

	return ret;
}
