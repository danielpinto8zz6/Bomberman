#include <stdbool.h>

void check_game_state();
void player_move(int move,int pid);
bool check_occupied(int pos_x,int pos_y);
int get_user_position(int pid);
coordinates get_player_position(int pid);
void set_player_position(int pid,int x,int y);
void *get_empty_positions(int count);
int get_empty_positions_number();
bool load_board(char *filename);
void place_objects();
void fill_board();
void load_game();
int get_game_objects();
int get_game_enemies();
void player_lost(int x,int y);
void enemy_lost(int x, int y);
int get_nr_objects();
void show_game_info();
coordinates get_first_empty_position_found();
void start_players_positions();
