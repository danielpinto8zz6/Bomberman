#include <ncurses.h>
#include <stdbool.h>

int main(int argc, char *argv[]);
void error(char *msg);
void SIGhandler(int sig);
int is_login_fields_valid(usersActive active);
void shutdown();
void *receiver(void *arg);
void forced_shutdown();
extern usersActive receive;
extern bool playing;
extern int logged;
extern WINDOW *info;
extern WINDOW *win;
