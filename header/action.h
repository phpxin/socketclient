#define CHAR_SIZE_1 200
#define TEXT_SIZE_1 65535

struct user{
	int id;
	char account[CHAR_SIZE_1];
	char pwd[CHAR_SIZE_1];
	char name[CHAR_SIZE_1];
	char avatar[CHAR_SIZE_1];
};

struct message{
	int id;
	int uid;
	int fid;
	char content[TEXT_SIZE_1];
	short type;
    time_t addtime;
};

void act_login();
void act_test();
void act_msg();
void recv_msg();
void act_user_message(const void *rmsg, size_t rmsg_len);

void update_user_link(int fd, int uid);
