#include "game.h"
#include <unistd.h>

void draw_borders(WINDOW *screen) {
  int x, y, i;

  getmaxyx(screen, y, x);

  mvwprintw(screen, 0, 0, "+");
  mvwprintw(screen, y - 1, 0, "+");
  mvwprintw(screen, 0, x - 1, "+");
  mvwprintw(screen, y - 1, x - 1, "+");

  for (i = 1; i < (y - 1); i++) {
    mvwprintw(screen, i, 0, "|");
    mvwprintw(screen, i, x - 1, "|");
  }

  for (i = 1; i < (x - 1); i++) {
    mvwprintw(screen, 0, i, "-");
    mvwprintw(screen, y - 1, i, "-");
  }
}

void game() {
  int parent_x, parent_y;
  int score_size = 5;
  int sidebar_size = 20;
  char command[50];

  initscr();
  curs_set(TRUE);

  getmaxyx(stdscr, parent_y, parent_x);

  WINDOW *content =
      newwin(parent_y - score_size, parent_x - sidebar_size, 0, 0);
  WINDOW *bottombar = newwin(score_size, parent_x, parent_y - score_size, 0);
  WINDOW *sidebar =
      newwin(parent_y - score_size, sidebar_size, 0, parent_x - sidebar_size);

  while (1) {

    wclear(stdscr);
    wclear(content);
    wclear(bottombar);
    wclear(sidebar);

    draw_borders(content);
    draw_borders(bottombar);
    draw_borders(sidebar);

    mvwprintw(content, 1, 1, "Field");
    mvwprintw(bottombar, 1, 1, "Commands:");
    mvwprintw(sidebar, 1, 1, "Active users");

    wrefresh(content);
    wrefresh(bottombar);
    wrefresh(sidebar);

    mvwscanw(bottombar, 2, 1, "%s", command);
  }

  endwin();
  return;
}
