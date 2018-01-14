#include <ncurses.h>

void create_board();
WINDOW *create_win(int height,int width,int starty,int startx);
void update_board(usersActive game);
void clean_board();
void place_in_board(int y,int x,char type);
