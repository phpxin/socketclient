struct user{
	int id;
	char account[200];
	char pwd[200];
	char name[200];
};

void act_login();
void act_test();
void act_msg();
void recv_msg();
void act_user_message(const void *rmsg, size_t rmsg_len);

void update_user_link(int fd, int uid);
