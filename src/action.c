#include "apue.h"
#include "net.h"
#include <string.h>
#include "menu.h"
#include "strutil.h"
#include "action.h"
#include "logicutil.h"
#include "protocol.h"

size_t pl_size = sizeof(int) ;
size_t protocol_size = sizeof(unsigned short) ;
size_t flag_size = sizeof(int) ;

extern int clientSockFlag;
extern struct user my;

static int msg_read(int fd, void **pkg, size_t *pkg_len);

void act_msg()
{
	if(my.id<=0){
		printf("please login \n");
		return ;
	}

	char content[65535] = {'\0'};

	printf("enter content:");
	fgets(content, 65534, stdin);
	su_trim(content, "\n\r");

	int content_len = strlen(content)+1; /* conent+\0 */

	int i_size = sizeof(int);
	int package_len = pl_size + protocol_size + i_size*3 + content_len;

	void *package = malloc(package_len);
	memset(package, '\0', package_len);
	
	int _package_len = int_to_net(package_len);
	unsigned short protocol = htons(PTO_MSG);
	int uid = int_to_net( my.id );
	int fid = int_to_net( 100 );
	int len = int_to_net( content_len );
	
	void *_package_cur = package;

	memcpy(_package_cur, &_package_len, pl_size);
	_package_cur += pl_size;
	memcpy(_package_cur, &protocol, protocol_size);
	_package_cur += protocol_size;
	memcpy(_package_cur, &uid, i_size);
	_package_cur += i_size;
	memcpy(_package_cur, &fid, i_size);
	_package_cur += i_size;
	memcpy(_package_cur, &len, i_size);
	_package_cur += i_size;
	memcpy(_package_cur, content, content_len);
	
	/* send */

	send(clientSockFlag, package, package_len, 0);
	
}

void act_login()
{
	int package_len = pl_size + protocol_size + 200*2;

	void *package = malloc(package_len);
	memset(package, '\0', package_len);
	
	int shift_p = 0;
	
	int _net_package_len = int_to_net(package_len);
	memcpy(package, &_net_package_len, pl_size) ;
	shift_p += pl_size;
	
	unsigned short protocol = PTO_LOGIN;
	unsigned short _net_protocol = htons(protocol);
	memcpy(package+shift_p, &_net_protocol, protocol_size);
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

	void *rmsg = NULL;
	size_t rmsg_len = 0;

	msg_read(clientSockFlag, &rmsg, &rmsg_len);

	/* 协议 + 状态 + 正文 */

	int s_protocol = 0;
	int s_flag = 0 ;


	memcpy(&s_protocol, rmsg, protocol_size );
	memcpy(&s_flag, rmsg+protocol_size, flag_size );
	
	s_protocol = ntohs(s_protocol) ;
	s_flag = net_to_int(s_flag);

	if(s_protocol == PTO_LOGIN){
		if(s_flag == 1){
			memcpy(&my, rmsg+protocol_size+flag_size, rmsg_len-protocol_size-flag_size);
			my.id = net_to_int(my.id);
			printf("uid: %d, name: %s \n", my.id, my.name);
		}else{
			printf("登录错误！\n");
		}
	}else{
		printf("协议不正确 \n");
	}

	if(rmsg != NULL){
		free(rmsg);
		rmsg = NULL;
	}
}

static int msg_read(int fd, void **pkg, size_t *pkg_len)
{
	
	int readlen = 0 , _rlen = 0;
	int _lenl = BUFSIZE;
	*pkg = malloc(_lenl);
	memset(*pkg, '\0', _lenl);

	int runflag = 1 , package_len = 0;
	int pkg_ll = sizeof(int); /* size of package_len */

	while( runflag ){

		_rlen = recv(fd, *pkg+readlen, _lenl, 0);
		readlen += _rlen;

		if(!package_len){
			memcpy(&package_len, *pkg, pkg_ll);
			package_len = net_to_int(package_len);
			printf("package len is %d \n", package_len);
		}

		if(_rlen >= _lenl && readlen < package_len){
			*pkg = realloc(*pkg, readlen+_lenl);
			memset(*pkg+readlen, '\0', _lenl);
			runflag = 1;
		}else{
			runflag = 0;
		}
	}

	if(readlen == 0)
	{
		printf("client %d was closed", fd);
		return 0;
	}

	if(readlen < 0)
	{
		printf("recv failed error num is %d", errno);
		return 0;
	}

	*pkg_len = readlen - pkg_ll;
	memcpy(*pkg, *pkg+pkg_ll, *pkg_len);
	
	return 1;
}

void act_test()
{

	printf("please enter string , and enter quit to exit . \n");

	while(1)
	{
		char sbuff[1024] = {'\0'} ;

		fgets(sbuff, 1023, stdin);

		if(strncmp(sbuff, "quit", 4) == 0)
		{
			break;
		}
		
		send(clientSockFlag, sbuff, 1024, 0);
	}
}

