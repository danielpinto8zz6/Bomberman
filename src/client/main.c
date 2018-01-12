#include "../structs.h"
#include "main.h"
#include <fcntl.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

WINDOW *win;

int logged = 0;

void place_in_board(int y, int x, char type) {
  x++;
  y++;
  mvwprintw(win, y, x, "%c", type);
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

  init_pair(1, COLOR_WHITE, COLOR_BLACK);

  attron(COLOR_PAIR(1));

  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  printw("Press q to exit");
  refresh();
  win = create_win(HEIGHT + 2, WIDTH + 2, 1, 1);
  curs_set(0);

  wrefresh(win);

  while ((ch = getch()) != 'q') {
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
    }
    wrefresh(win);
  }

  endwin();
}

void forced_shutdown() {
  char pipe[10];
  sprintf(pipe, "pipe-%d", getpid());
  unlink(pipe);
  printf("Programa terminado\n");
  endwin();
  exit(0);
}

void *receiver(void *arg) {
  char pipe[10];
  int fd_pipe, stop = 0;

  usersActive receive;

  sprintf(pipe, "pipe-%d", getpid());
  mkfifo(pipe, 0600);

  fd_pipe = open(pipe, O_RDWR);

  do {
    read(fd_pipe, &receive, sizeof(receive));

    switch (receive.action) {
    case SERVER_SHUTDOWN: /* exit */
      printf("O servidor encerrou, a fechar pipe e sair...\n");
      forced_shutdown();
      break;
    case KICK: /* kick */
      printf("O servidor kickou-o, a fechar pipe e sair...\n");
      forced_shutdown();
      break;
    case LOGGED: /* Login sucefully */
      // printf("Sessão iniciada com sucesso\n");
      logged = 1;
      break;
    case WRONG_CREDENTIALS: /* User or pass wrong*/
      printf("Username ou password errados\n");
      break;
    case ALREADY_LOGGED: /* user already logged */
      printf("O utilizadorja está logado\n");
    case UPDATE: /* Update Board */
      update_board(receive);
    default:
      break;
    }
  } while (stop == 0);

  close(fd_pipe);
  unlink(pipe);
  pthread_exit(0);
}

int is_login_fields_valid(usersActive active) {
  int i;
  int status = 1;

  for (i = 0; active.username[i] != '\0'; i++) {
    if (active.username[i] == ' ')
      status = 0;
  }

  for (i = 0; active.password[i] != '\0'; i++) {
    if (active.password[i] == ' ')
      status = 0;
  }

  if (status == 0)
    printf("O username e a password não podem conter espaços!\n\n");

  return status;
}

void shutdown() {
  char pipe[10];
  int fd;
  usersActive send;
  if (logged == 1) {
    send.pid = getpid();
    send.action = 2;
    sprintf(pipe, "pipe-%d", getpid());
    fd = open(PIPE, O_WRONLY, 0600);
    write(fd, &send, sizeof(send));
    endwin();
    unlink(pipe);
    printf("Programa terminado\n");
    exit(0);
  } else {
    forced_shutdown();
  }
}

void SIGhandler(int sig) {
  signal(sig, SIG_IGN);
  shutdown();
}

void error(char *msg) { printf("%s", msg); }

int main(int argc, char *argv[]) {
  int fd, res;
  char pipe[20];
  pthread_t task;
  usersActive send;

  signal(SIGINT, SIGhandler);

  setbuf(stdout, NULL);

  if (access(PIPE, F_OK) != 0) {
    error("O servidor não se encontra em execução. A sair...\n");
    exit(0);
  }

  send.pid = getpid();
  sprintf(pipe, "pipe-%d", send.pid);

  if (mkfifo(pipe, S_IRWXU) < 0) {
    error("Erro ao criar pipe. A sair...\n");
    exit(0);
  }

  res = pthread_create(&task, NULL, &receiver, NULL);

  if (res != 0) {
    perror("ERRO!A criar a thread!!!\n");
    unlink(pipe);
    exit(1);
  }

  fd = open(PIPE, O_RDWR);

  if (fd == -1) {
    error("Erro ao criar pipe. A sair...\n");
    exit(0);
  }
  do {
    do {
      printf("\n#### LOGIN ####\n");
      printf("\nUsername : ");
      scanf(" %19[^\n]s", send.username);
      printf("\nPassword : ");
      scanf(" %19[^\n]s", send.password);
    } while (is_login_fields_valid(send) == 0);
    if (access(PIPE, F_OK) != 0) {
      error("O servidor não se encontra em execução. A sair...\n");
      exit(0);
    }
    send.action = LOGIN;
    write(fd, &send, sizeof(send));
    sleep(2);
  } while (logged == 0);
  if (logged == 1)
    create_board();

  pthread_join(task, NULL);
  close(fd);
  unlink(pipe);

  return 0;
}
