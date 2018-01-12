/* This file was automatically generated.  Do not edit! */
int main(int argc,char *argv[]);
void error(char *msg);
void keyboard(char *cmd);
void SIGhandler(int sig);
void shutdown();
void *receiver(void *arg);
void kick_user(char username[20]);
extern usersActive active_user[20];
extern int nr_active_users;
extern users user[20];
extern int nr_users;
