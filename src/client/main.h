int main(int argc,char *argv[]);
void error(char *msg);
void SIGhandler(int sig);
void shutdown();
int is_login_fields_valid(usersActive active);
void *receiver(void *arg);
