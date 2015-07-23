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

extern int clientSockFlag;

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

