#include "../structs.h"
#include "game.h"
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
WINDOW *info;

int logged = 0;

bool playing = true;

usersActive receive;

void forced_shutdown() {
  char pipe[10];
  sprintf(pipe, "pipe-%d", getpid());
  unlink(pipe);
  printw("Programa terminado! Pressione qualquer tecla para sair\n");
  refresh();
  getch();
  endwin();
  exit(0);
}

void *receiver(void *arg) {
  char pipe[10];
  int fd_pipe, stop = 0;

  sprintf(pipe, "pipe-%d", getpid());
  mkfifo(pipe, 0600);

  fd_pipe = open(pipe, O_RDWR);

  do {
    read(fd_pipe, &receive, sizeof(receive));

    switch (receive.action) {
    case SERVER_SHUTDOWN: /* exit */
      clear();
      printw("O servidor encerrou, a fechar pipe e sair...\n");
      refresh();
      forced_shutdown();
      break;
    case KICK: /* kick */
      clear();
      printw("O servidor kickou-o, a fechar pipe e sair...\n");
      forced_shutdown();
      break;
    case MAX_ACTIVE_USERS_EXCEEDED: /* Cant login, max exceeded */
      clear();
      printw("O utilizador nao pode iniciar sessao pois ultrapassa o numero "
             "maximo de utilizadores ativos\n");
      shutdown();
      break;
    case LOGGED: /* Login sucefully */
      logged = 1;
      break;
    case WRONG_CREDENTIALS: /* User or pass wrong*/
      printw("Username ou password errados\n");
      refresh();
      break;
    case ALREADY_LOGGED: /* user already logged */
      printw("O utilizador ja esta logado\n");
      refresh();
    case UPDATE: /* Update Board */
      if (receive.playing == LOST)
        playing = false;
      update_board(receive);
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

  if (status == 0) {
    printw("O username e a password não podem conter espaços!\n\n");
    refresh();
  }

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
    unlink(pipe);
    clear();
    printw("Programa terminado! Pressione qualquer tecla para sair\n");
    refresh();
    getch();
    endwin();
    exit(0);
  } else {
    forced_shutdown();
  }
}

void SIGhandler(int sig) {
  signal(sig, SIG_IGN);
  shutdown();
}

void error(char *msg) {
  printw("%s", msg);
  refresh();
}

int main(int argc, char *argv[]) {
  int fd, res;
  char pipe[20];
  pthread_t task;
  usersActive send;

  signal(SIGINT, SIGhandler);
  signal(SIGUSR1, SIGhandler);
  signal(SIGHUP, SIGhandler);

  setbuf(stdout, NULL);

  initscr();

  start_color();

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLUE);
  init_pair(3, COLOR_WHITE, COLOR_RED);
  init_pair(4, COLOR_WHITE, COLOR_GREEN);
  init_pair(5, COLOR_WHITE, COLOR_YELLOW);
  init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(7, COLOR_WHITE, COLOR_CYAN);
  init_pair(8, COLOR_BLUE, COLOR_WHITE);

  if (access(PIPE, F_OK) != 0) {
    error("O servidor nao se encontra em execucao. A sair...\n");
    getch();
    endwin();
    exit(0);
  }

  send.pid = getpid();
  sprintf(pipe, "pipe-%d", send.pid);

  if (mkfifo(pipe, S_IRWXU) < 0) {
    error("Erro ao criar pipe. A sair...\n");
    getch();
    endwin();
    exit(0);
  }

  res = pthread_create(&task, NULL, &receiver, NULL);

  if (res != 0) {
    perror("ERRO!A criar a thread!!!\n");
    unlink(pipe);
    getch();
    endwin();
    exit(1);
  }

  fd = open(PIPE, O_RDWR);

  if (fd == -1) {
    error("Erro ao criar pipe. A sair...\n");
    getch();
    endwin();
    exit(0);
  }
  do {
    do {
      printw("\n#### LOGIN ####\n");
      printw("\nUsername : ");
      refresh();
      scanw(" %19[^\n]s", send.username);
      printw("\nPassword : ");
      refresh();
      scanw(" %19[^\n]s", send.password);
    } while (is_login_fields_valid(send) == 0);
    if (access(PIPE, F_OK) != 0) {
      error("O servidor nao se encontra em execucao. A sair...\n");
      exit(0);
    }
    send.action = LOGIN;
    write(fd, &send, sizeof(send));
    sleep(2);
  } while (logged == 0);
  if (logged == 1) {
    endwin();
    clear();
    create_board();
  }

  pthread_join(task, NULL);
  close(fd);
  unlink(pipe);

  return 0;
}
