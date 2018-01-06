#include <ncurses.h>
void create_board();
WINDOW *create_win(int height, int width, int starty, int startx);
int check_occupied(int x, int y);
void place_object(const char *type, int x, int y);
extern WINDOW *win;
