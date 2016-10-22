/**
 * chat 客户端
 */

#include "apue.h"
#include "net.h"
#include <string.h>
#include "menu.h"
#include "strutil.h"
#include "action.h"
/*
#include "sys/epoll.h"
*/
#include <event.h>

#define MAX_EFDS 1	/* only test client readable */

static int print_menu();
static void *thread_func(void *udata);
void onRead(int client, short iEvent, void *arg) ;
static void sig_func(int signum); /* 信号处理 */

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

	int flag = 0 ;
	
	/*
	int flag = pthread_create(&thread_c1, NULL, thread_func, NULL);
	if(flag != 0)
	{
		printf("run child thread failed error num is %d", errno);
		exit( -1 );
	}
	*/

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

	/*
	ep_fd = epoll_create(5);
	
	struct epoll_event _event;
	_event.data.fd = clientSockFlag;
    _event.events = EPOLLIN ;
	*/

	
	
	flag = pthread_create(&thread_c1, NULL, thread_func, (void *)(&clientSockFlag));
	
	if(flag != 0)
	{
		printf("run child thread failed error num is %d", errno);
		exit( -1 );
	}
	
	/*
	创建一个线程默认的状态是joinable, 如果一个线程结束运行但没有被join,则它的状态类似于进程中的Zombie Process,
	即还有一部分资源没有被回收（退出状态码），所以创建线程者应该pthread_join来等待线程运行结束，
	并可得到线程的退出代码，回收其资源（类似于wait,waitpid)
	*/
	pthread_detach(thread_c1);
	

	/*
	flag = epoll_ctl(ep_fd, EPOLL_CTL_ADD, clientSockFlag, &_event);
    if (flag == -1) {
		printf("epoll_ctl failed error num is %d", errno);
		close(clientSockFlag);
		close(ep_fd);
        exit( -1 );
    }
    */

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
	/*close(ep_fd);*/

	exit(EXIT_SUCCESS);
}

void onRead(int client, short iEvent, void *arg)
{

	recv_msg();
}

static void *thread_func(void *udata)
{

	/*
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
	
	signal(SIGKILL,sig_func);
	*/
	
	printf("socket client fd is %d \n", *((int *)udata));
	
	struct event_base* client_ebase;
	
	client_ebase = event_base_new();  /* 创建一个 event 的描述符 */
	
	/* 连接注册为新事件 (EV_PERSIST为事件触发后不默认删除)   sizeof(struct event) */
	struct event *pEvRead = (struct event *)malloc( sizeof(struct event) );
	event_set(pEvRead, *((int *)udata), EV_READ|EV_PERSIST, onRead, NULL);
	/*event_set(pEvRead, clientSockFlag, EV_READ|EV_PERSIST, onRead, NULL);*/
	event_base_set(client_ebase, pEvRead);
	int flag = event_add(pEvRead, NULL);
	
	if(flag == -1){
		printf("event_add failed \n");
		perror("event_add");
	}
	
	event_base_dispatch(client_ebase); /* 开始事件循环 */

	


	pthread_exit( NULL );
	
}

static void sig_func(int signum)
{
	printf("recv signal %d\n", signum);
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

	if( my.id > 0 )
	{
		printf("My Info --- uid: %d, name: %s \n", my.id, my.name);
	}else{
		printf("No login !\n");
	}

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
