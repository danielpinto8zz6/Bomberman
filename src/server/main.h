#include <stdbool.h>
#include <pthread.h>
int main(int argc,char *argv[]);
void error(char *msg);
void keyboard(char *cmd);
void SIGhandler(int sig);
void shutdown();
void *receiver(void *arg);
void send_update(int pid);
void update_all_users();
void kick_user(char username[20]);
int random_number(int limit);
bool check_if_users_exceeds_max_active();
extern pthread_t thread;
extern pthread_t thread_bombs;
extern pthread_mutex_t lock;
extern pthread_t enemy_thread[20];
extern Board b;
extern bool map_loaded;
extern int game;
extern int nr_enemies;
extern coordinates enemy[20];
extern usersActive active_user[20];
extern int nr_active_users;
extern users user[20];
extern int nr_users;
extern int game_exit;
