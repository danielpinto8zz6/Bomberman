#include <stdbool.h>
int main(int argc,char *argv[]);
void error(char *msg);
void keyboard(char *cmd);
void SIGhandler(int sig);
void shutdown();
void *receiver(void *arg);
coordinates get_first_empty_position_found();
void send_update(int pid);
void kick_user(char username[20]);
void player_move(int move,int pid);
bool check_occupied(int pos_x,int pos_y);
int get_user_position(int pid);
coordinates get_player_position(int pid);
void set_player_position(int pid,int x,int y);
void load_game();
void fill_board();
void place_objects();
void *get_empty_positions(int count);
int get_empty_positions_number();
bool load_board(char *filename);
int get_game_objects();
int get_game_enemies();
int random_number(int limit);
bool check_if_users_exceeds_max_active();
void start_players_positions();
void place_bomb(Bomb *bomb);
void update_all_users();
void *thread_bomb(void *arg);
extern Board b;
extern bool map_loaded;
extern int game;
extern usersActive active_user[20];
extern int nr_active_users;
extern users user[20];
extern int nr_users;
