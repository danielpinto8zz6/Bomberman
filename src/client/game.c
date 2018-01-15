#include "../structs.h"
#include "game.h"
#include "main.h"
#include <fcntl.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void place_in_board(int y, int x, char type) {
  x++;
  y++;
  switch (type) {
  case '*':
    wattron(win, COLOR_PAIR(2));
    break;
  case 'X':
  case 'x':
    wattron(win, COLOR_PAIR(3));
    break;
  case '#':
    wattron(win, COLOR_PAIR(4));
    break;
  case '&':
    wattron(win, COLOR_PAIR(5));
    break;
  case 'O':
  case 'b':
  case 'C':
  case 'B':
  case 'E':
    wattron(win, COLOR_PAIR(6));
    break;
  case '$':
    wattron(win, COLOR_PAIR(7));
    break;
  case 'S':
    wattron(win, COLOR_PAIR(3));
    break;
  }
  mvwprintw(win, y, x, "%c", type);
  wattron(win, COLOR_PAIR(1));
}

void clean_board() {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      place_in_board(y, x, ' ');
    }
  }
}

void update_board(usersActive game) {
  clean_board();
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      place_in_board(y, x, game.board.board[y][x]);
    }
  }
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (game.board.bombs[y][x] == 'x' || game.board.bombs[y][x] == 'X')
        place_in_board(y, x, game.board.bombs[y][x]);
    }
  }
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (game.board.users[y][x] == '*')
        place_in_board(y, x, game.board.users[y][x]);
    }
  }
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (game.board.enemies[y][x] == '$')
        place_in_board(y, x, game.board.enemies[y][x]);
    }
  }

  if (game.playing == PLAYING) {
    wattron(win, COLOR_PAIR(8));
    mvwprintw(win, game.y + 1, game.x + 1, "%c", '*');
    wattron(win, COLOR_PAIR(1));
  }

  mvwprintw(info, 3, 9, "\t");
  mvwprintw(info, 5, 14, "\t");
  mvwprintw(info, 7, 13, "\t");
  mvwprintw(info, 9, 9, "\t\t");
  mvwprintw(info, 3, 9, "%d", game.pontuation);
  mvwprintw(info, 5, 14, "%d", game.bigbombs);
  mvwprintw(info, 7, 13, "%d", game.minibombs);
  if (game.playing == PLAYING)
    mvwprintw(info, 9, 9, "Em jogo");
  else if (game.playing == LOST)
    mvwprintw(info, 9, 9, "Perdeu");
  wrefresh(info);
  wrefresh(win);
}

WINDOW *create_win(int height, int width, int starty, int startx) {
  WINDOW *local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);
  wrefresh(local_win);

  return (local_win);
}

void create_board() {
  usersActive game;

  game.pid = getpid();

  int fd = open(PIPE, O_RDWR);

  int ch;

  initscr();

  start_color();

  attron(COLOR_PAIR(1));

  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  printw("Press q to exit");
  refresh();
  win = create_win(HEIGHT + 2, WIDTH + 2, 1, 1);

  info = newwin(HEIGHT + 2, 50, 1, WIDTH + 4);

  wrefresh(info);

  curs_set(0);

  wrefresh(win);
  wrefresh(info);

  mvwprintw(info, 1, 1, "Score");
  mvwprintw(info, 3, 1, "Pontos:");
  mvwprintw(info, 5, 1, "Mega Bombas:");
  mvwprintw(info, 7, 1, "Bombinnhas:");
  mvwprintw(info, 9, 1, "Estado:");
  mvwprintw(info, 12, 1, "Para mover o jogador utilize as setas");
  mvwprintw(info, 14, 1, "Para colocar uma mega bomba utilize o M");
  mvwprintw(info, 16, 1, "Para colocar uma bombinha utilize o B");

  while ((ch = getch()) != 'q') {
    if (playing == true) {
      switch (ch) {
      case KEY_LEFT:
        game.action = LEFT;
        write(fd, &game, sizeof(game));
        break;
      case KEY_RIGHT:
        game.action = RIGHT;
        write(fd, &game, sizeof(game));
        break;
      case KEY_UP:
        game.action = UP;
        write(fd, &game, sizeof(game));
        break;
      case KEY_DOWN:
        game.action = DOWN;
        write(fd, &game, sizeof(game));
        break;
      case 'm':
      case 'M':
        game.action = BIGBOMB;
        write(fd, &game, sizeof(game));
        break;
      case 'b':
      case 'B':
        game.action = MINIBOMB;
        write(fd, &game, sizeof(game));
        break;
      }
    }
    wrefresh(win);
  }
  shutdown();
}
