#include "../structs.h"
#include "activeusers.h"
#include "game.h"
#include "login.h"
#include "main.h"
#include "users.h"
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int nr_users;
users user[20];
int nr_active_users;
usersActive active_user[20];

Board b;

typedef struct {
  int x;
  int y;
} coordinates;

void fill_board() {
  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
      b.board[y][x] = ' ';
}

void set_player_position(int pid, int x, int y) {
  for (int i = 0; i < nr_active_users; i++) {
    if (active_user[i].pid == pid) {
      active_user[i].x = x;
      active_user[i].y = y;
      b.board[y][x] = '*';
    }
  }
}

coordinates get_player_position(int pid) {
  coordinates pos;
  for (int i = 0; i < nr_active_users; i++) {
    if (active_user[i].pid == pid) {
      pos.x = active_user[i].x;
      pos.y = active_user[i].y;
    }
  }
  return pos;
}

int get_user_position(int pid) {
  int i = 0;
  for (i = 0; i < nr_active_users; i++) {
    if (pid == active_user[i].pid) {
      return i;
    }
  }
  return -1;
}

bool check_occupied(int pos_x, int pos_y) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (pos_x == x && pos_y == y) {
        if (b.board[y][x] == '#' || b.board[y][x] == '*') {
          return true;
        }
      }
    }
  }
  return false;
}

void player_move(int move, int pid) {
  coordinates pos = get_player_position(pid);
  int i = get_user_position(pid);
  int x = pos.x;
  int y = pos.y;

  switch (move) {
  case UP:
    /* y-- */
    y--;
    if (y >= 0 && check_occupied(x, y) == false) {
      b.board[y][x] = '*';
      b.board[pos.y][x] = ' ';
      active_user[i].y = y;
    }
    break;
  case DOWN:
    /* y++ */
    y++;
    if (y < HEIGHT && check_occupied(x, y) == false) {
      b.board[y][x] = '*';
      b.board[pos.y][x] = ' ';
      active_user[i].y = y;
    }
    break;
  case LEFT:
    /* x-- */
    x--;
    if (x >= 0 && check_occupied(x, y) == false) {
      b.board[y][x] = '*';
      b.board[y][pos.x] = ' ';
      active_user[i].x = x;
    }
    break;
  case RIGHT:
    /* x++ */
    x++;
    if (x < WIDTH && check_occupied(x, y) == false) {
      b.board[y][x] = '*';
      b.board[y][pos.x] = ' ';
      active_user[i].x = x;
    }
    break;
  }
}

void kick_user(char username[20]) {
  char pipe[10];
  int fd, found = 0;
  usersActive send;

  send.action = KICK;

  for (int i = 0; i < nr_active_users; i++) {
    if (strcmp(active_user[i].username, username) == 0) {
      sprintf(pipe, "pipe-%d", active_user[i].pid);
      delete_from_active_users_list(active_user[i].pid);
      found = 1;
    }
  }
  if (found == 1) {
    fd = open(pipe, O_WRONLY, 0600);
    write(fd, &send, sizeof(send));
    close(fd);
  } else {
    printf("Utilizador não encontrado");
  }
}

void send_update(int pid) {
  usersActive send;
  int fd;
  char pipe[10];

  send.action = UPDATE;
  send.board = b;

  sprintf(pipe, "pipe-%d", pid);
  fd = open(pipe, O_WRONLY, 0600);

  write(fd, &send, sizeof(send));

  close(fd);
}

void update_all_users() {
  if (nr_active_users > 0) {
    for (int i = 0; i < nr_active_users; i++) {
      send_update(active_user[i].pid);
    }
  }
}

void *receiver(void *arg) {
  usersActive receive;
  usersActive send;
  int fd, fd_send, stop = 0;
  char pipe[10];

  mkfifo(PIPE, 0600);
  fd = open(PIPE, O_RDWR);

  do {
    read(fd, &receive, sizeof(receive));

    sprintf(pipe, "pipe-%d", receive.pid);
    fd_send = open(pipe, O_WRONLY, 0600);

    switch (receive.action) {
    case LOGIN:
      if (check_if_user_is_logged(receive.username) == 0) {
        if (validate(receive.username, receive.password) == 1) {
          printf("\n%d -> %s iniciou sessão\n", receive.pid, receive.username);
          add_to_active_users_list(receive.pid, receive.username);
          set_player_position(receive.pid, 5, 5);
          coordinates pos = get_player_position(receive.pid);
          send.x = pos.x;
          send.y = pos.y;
          send.action = LOGGED;
        } else {
          send.action = WRONG_CREDENTIALS;
        }
      } else {
        send.action = ALREADY_LOGGED;
      }
      write(fd_send, &send, sizeof(send));
      break;
    case LOGOUT:
      if (check_if_user_is_logged(get_username_from_pid(receive.pid)) == 1) {
        /* User left */
        printf("O cliente %d -> %s saiu do jogo!\n", receive.pid,
               get_username_from_pid(receive.pid));
        delete_from_active_users_list(receive.pid);
      }
    case UP:
      player_move(UP, receive.pid);
      update_all_users();
      break;
    case DOWN:
      player_move(DOWN, receive.pid);
      update_all_users();
      break;
    case LEFT:
      player_move(LEFT, receive.pid);
      update_all_users();
      break;
    case RIGHT:
      player_move(RIGHT, receive.pid);
      update_all_users();
      break;
    }

  } while (stop == 0);
  close(fd);
  pthread_exit(0);
}

void shutdown() {
  char pipe[10];
  int fd;
  usersActive send;

  send.action = SERVER_SHUTDOWN;

  for (int i = 0; i < nr_active_users; i++) {
    sprintf(pipe, "pipe-%d", active_user[i].pid);
    fd = open(pipe, O_WRONLY, 0600);
    write(fd, &send, sizeof(send));
    close(fd);
  }
  unlink(PIPE);
  printf("Programa terminado\n");
  exit(0);
}

void SIGhandler(int sig) {
  signal(sig, SIG_IGN);
  shutdown();
}

void keyboard(char *cmd) {
  char **arg = NULL;
  char *p = strtok(cmd, " ");
  int n_spaces = 0;

  /* dividir a string e anexar tokens em 'arg' */
  while (p) {
    arg = realloc(arg, sizeof(char *) * ++n_spaces);
    if (arg == NULL)
      exit(-1); /* falha na alocação de meḿória */
    arg[n_spaces - 1] = p;
    p = strtok(NULL, " ");
  }

  /* Realocar um elemento extra para o último NULL */
  arg = realloc(arg, sizeof(char *) * (n_spaces + 1));
  arg[n_spaces] = 0;

  if (strcmp(arg[0], "add") == 0) {
    if (arg[1] != NULL && arg[2] != NULL) {
      add_user(arg[1], arg[2]);
    } else
      printf("Faltam argumentos\n");
  } else if (strcmp(arg[0], "shutdown") == 0) {
    shutdown();
  } else if (strcmp(arg[0], "users") == 0) {
    print_active_users_list();
  } else if (strcmp(arg[0], "help") == 0) {
    printf("add [username][password] -> adiciona um utilizador\n");
    printf("shutdown -> termina o servidor\n");
    printf("users -> mostra os utilizadores ativos\n");
    printf("registered -> mostra os utilizadores registados\n");
  } else if (strcmp(arg[0], "registered") == 0) {
    printf("Registered users : \n\n");
    print_users_list();
  } else if (strcmp(arg[0], "kick") == 0) {
    if (arg[1] != NULL) {
      kick_user(arg[1]);
    } else
      printf("Faltam argumentos\n");
  } else {
    printf("Comando inválido!\n");
    printf("Insira <help> para ver a lista de comandos disponíveis\n");
  }
  free(arg);
}

void error(char *msg) { printf("%s", msg); }

int main(int argc, char *argv[]) {
  pthread_t thread;
  char cmd[80];
  int fd_pipe, res;
  usersActive active;

  fill_board();

  signal(SIGINT, SIGhandler);

  setbuf(stdout, NULL);

  if (access(PIPE, F_OK) == 0) {
    error("Já se encontra um servidor em execução. A sair...\n");
    exit(0);
  }

  if (mkfifo(PIPE, S_IRWXU) < 0) {
    error("Erro ao criar pipe. A sair...\n");
    exit(0);
  }

  fd_pipe = open(PIPE, O_RDWR);

  if (fd_pipe == -1) {
    error("Erro ao abrir ficheiro. A sair...\n");
    exit(0);
  }

  printf("Servidor iniciado!\n");

  res = pthread_create(&thread, NULL, &receiver, NULL);

  if (res != 0) {
    perror("ERRO! A criar a thread!!!\n");
    write(fd_pipe, &active, sizeof(active));
    close(fd_pipe);
    unlink(PIPE);
    exit(1);
  }

  /* Carregar utilizadores */
  loadusers();

  while (1) {
    scanf(" %79[^\n]s", cmd);
    if (cmd[strlen(cmd) - 1] == '\n')
      cmd[strlen(cmd) - 1] = '\0';
    if (strcmp(cmd, " ") != 0)
      keyboard(cmd);
    // printf("Comando: %s\n", cmd);
  }
  write(fd_pipe, &active, sizeof(active));
  pthread_join(thread, NULL);
  close(fd_pipe);
  unlink(PIPE);
  return 0;
}
