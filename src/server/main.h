int main(int argc,char *argv[]);
void error(char *msg);
void keyboard(char *cmd);
void SIGhandler(int sig);
void shutdown();
void *receiver(void *arg);
