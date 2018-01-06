#include "../structs.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WINDOW *win;

#define WIDTH 30
#define HEIGHT 20

int array_size(int array[]) {
  int i;
  for (i = 0; array[i] != '\0'; i++)
    ;
  return i;
}

int *join_array(int arr1[], int arr2[]) {
  memcpy(arr1 + array_size(arr1), arr2, array_size(arr2));
  return arr1;
}

void place_object(const char *type, int x, int y) {
  mvwprintw(win, y, x, type);
}

int *get_occupied_positions_x() {
  int *occupied;
  occupied = join_array(board.block_x, board.exit_x);
  occupied = join_array(occupied, board.object_x);
  occupied = join_array(occupied, board.walls_x);
  occupied = join_array(occupied, board.miniBomb_x);
  occupied = join_array(occupied, board.bigBomb_x);
  occupied = join_array(occupied, board.enemies_x);
  occupied = join_array(occupied, board.colector_x);
  occupied = join_array(occupied, board.extra_x);

  return occupied;
}

int *get_occupied_positions_y() {
  int *occupied;
  occupied = join_array(board.block_y, board.exit_y);
  occupied = join_array(occupied, board.object_y);
  occupied = join_array(occupied, board.walls_y);
  occupied = join_array(occupied, board.miniBomb_y);
  occupied = join_array(occupied, board.bigBomb_y);
  occupied = join_array(occupied, board.enemies_y);
  occupied = join_array(occupied, board.colector_y);
  occupied = join_array(occupied, board.extra_y);

  return occupied;
}

int check_occupied(int x, int y) {
  int *occupied_x = get_occupied_positions_x();
  int *occupied_y = get_occupied_positions_y();
  for (int i = 0; i < array_size(occupied_x); i++) {
    if (occupied_x[i] == x && occupied_y[i] == y) {
      return 1;
    }
  }
  return 0;
}

WINDOW *create_win(int height, int width, int starty, int startx) {
  WINDOW *local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);
  wrefresh(local_win);

  return (local_win);
}

void create_board() {
  int ch;
  int x = 1;
  int y = 1;

  initscr();

  start_color();

  init_pair(1, COLOR_WHITE, COLOR_BLACK);

  attron(COLOR_PAIR(1));

  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  printw("Press q to exit");
  refresh();
  win = create_win(HEIGHT + 2, WIDTH + 2, y, x);

  wmove(win, y, x);
  wrefresh(win);

  while ((ch = getch()) != 'q') {
    switch (ch) {
    case KEY_LEFT:
      if (x > 1)
        if (check_occupied(x - 1, y) == 0)
          x--;
      break;
    case KEY_RIGHT:
      if (x < WIDTH)
        if (check_occupied(x + 1, y) == 0)
          x++;
      break;
    case KEY_UP:
      if (y > 1)
        if (check_occupied(x, y - 1) == 0)
          y--;
      break;
    case KEY_DOWN:
      if (y < HEIGHT)
        if (check_occupied(x, y + 1) == 0)
          y++;
      break;
    }

    wmove(win, y, x);
    wrefresh(win);
  }

  endwin();
}
