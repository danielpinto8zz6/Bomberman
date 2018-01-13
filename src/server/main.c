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

int game = 0;

bool map_loaded = false;

Board b;

typedef struct {
  int x;
  int y;
} coordinates;

void start_players_positions() {
  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
      b.users[y][x] = ' ';
}

bool check_if_users_exceeds_max_active() {
  const char *env = getenv("NMAXPLAY");
  int max = 0;
  if (env != NULL)
    max = atoi(env);
  else
    max = 20;

  if (nr_active_users + 1 > max)
    return true;
  else
    return false;
}

int random_number(int limit) {
  int divisor = RAND_MAX / (limit + 1);
  int retval;

  do {
    retval = rand() / divisor;
  } while (retval > limit);

  return retval;
}

int get_game_enemies() {
  int enermies = 0;
  const char *env = getenv("NENEMY");
  if (env != NULL) {
    enermies = atoi(env) + game;
  } else {
    enermies = random_number(50) + game;
  }
  return enermies;
}

int get_game_objects() {
  int objects = 0;
  const char *env = getenv("NOBJECT");
  if (env != NULL) {
    objects = atoi(env) + game;
  } else {
    objects = random_number(50) + game;
  }
  return objects;
}

bool load_board(char *filename) {
  FILE *f;
  char c;
  int y = 0, x = 0;

  f = fopen(filename, "r");

  if (!f) {
    return false;
  } else {
    while ((c = fgetc(f)) != EOF) {

      if (c == '\n') {
        y++;
        x = 0;
      } else {
        b.board[y][x] = c;
        x++;
      }
    }
  }
  return true;
}

int get_empty_positions_number() {
  int i = 0;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (b.board[y][x] == ' ') {
        i++;
      }
    }
  }
  return i;
}

void *get_empty_positions(int count) {
  coordinates *empty = NULL;
  empty = malloc(sizeof(empty) * count);

  int i = 0;

  if (!empty)
    return NULL;

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (b.board[y][x] == ' ') {
        empty[i].x = x;
        empty[i].y = y;
        i++;
      }
    }
  }

  return empty;
}

void place_objects() {
  int objects = get_game_objects();

  int x = 0;
  int y = 0;
  int random = 0;

  for (int j = 0; j < objects; j++) {
    int i = get_empty_positions_number();
    coordinates *empty_positions = get_empty_positions(i);
    if (empty_positions) {
      free(empty_positions);
    }
    random = random_number(i);
    x = empty_positions[random].x;
    y = empty_positions[random].y;
    b.board[y][x] = 'O';
  }
}

void fill_board() {
  if (map_loaded == true) {
    return;
  } else {
    char board[HEIGHT][WIDTH] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '&', '&', '&', '&', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '&', '&', '&', '&', '&', ' ',
        ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#',
        '#', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', '&', '&', ' ', ' ', ' ', ' ',
        '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
        ' ', ' ', '&', '&', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '&', '&', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    };

    for (int y = 0; y < HEIGHT; y++)
      for (int x = 0; x < WIDTH; x++)
        b.board[y][x] = board[y][x];
  }
}

void load_game() {
  start_players_positions();
  fill_board();
  place_objects();
}

void set_player_position(int pid, int x, int y) {
  for (int i = 0; i < nr_active_users; i++) {
    if (active_user[i].pid == pid) {
      active_user[i].x = x;
      active_user[i].y = y;
      b.users[y][x] = '*';
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
        if (b.board[y][x] == '#' || b.board[y][x] == '&' || b.users[y][x] == '*') {
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
      if (b.board[y][x] == 'O') {
        active_user[i].pontuation++;
        b.board[y][x] = ' ';
      }
      b.users[y][x] = '*';
      b.users[pos.y][x] = ' ';
      active_user[i].y = y;
    }
    break;
  case DOWN:
    /* y++ */
    y++;
    if (y < HEIGHT && check_occupied(x, y) == false) {
      if (b.board[y][x] == 'O') {
        active_user[i].pontuation++;
        b.board[y][x] = ' ';
      }
      b.users[y][x] = '*';
      b.users[pos.y][x] = ' ';
      active_user[i].y = y;
    }
    break;
  case LEFT:
    /* x-- */
    x--;
    if (x >= 0 && check_occupied(x, y) == false) {
      if (b.board[y][x] == 'O') {
        active_user[i].pontuation++;
        b.board[y][x] = ' ';
      }
      b.users[y][x] = '*';
      b.users[y][pos.x] = ' ';
      active_user[i].x = x;
    }
    break;
  case RIGHT:
    /* x++ */
    x++;
    if (x < WIDTH && check_occupied(x, y) == false) {
      if (b.board[y][x] == 'O') {
        active_user[i].pontuation++;
        b.board[y][x] = ' ';
      }
      b.users[y][x] = '*';
      b.users[y][pos.x] = ' ';
      active_user[i].x = x;
    }
    break;
  case BIGBOMB:
    if (active_user[i].bigbombs > 0) {
      active_user[i].bigbombs--;
      b.board[y][x] = 'B';
    }
    break;
  case MINIBOMB:
    if (active_user[i].minibombs > 0) {
      active_user[i].minibombs--;
      b.board[y][x] = 'b';
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

  int i = get_user_position(pid);

  send.action = UPDATE;
  send.board = b;
  send.pontuation = active_user[i].pontuation;
  send.minibombs = active_user[i].minibombs;
  send.bigbombs = active_user[i].bigbombs;

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

coordinates get_first_empty_position_found() {
  coordinates pos;
  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
      if (b.board[y][x] == ' ') {
        pos.y = y;
        pos.x = x;
        break;
      }
  return pos;
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
        if (check_if_users_exceeds_max_active() == false) {
          if (validate(receive.username, receive.password) == 1) {
            printf("\n%d -> %s iniciou sessão\n", receive.pid,
                   receive.username);
            add_to_active_users_list(receive.pid, receive.username);

            /* To set player position when he logs, choose first empty position
             * found */
            coordinates c = get_first_empty_position_found();
            set_player_position(receive.pid, c.x, c.y);
            coordinates pos = get_player_position(receive.pid);
            active_user[get_user_position(receive.pid)].pontuation = 0;
            active_user[get_user_position(receive.pid)].minibombs = 3;
            active_user[get_user_position(receive.pid)].bigbombs = 2;
            send.x = pos.x;
            send.y = pos.y;
            send.action = LOGGED;
          } else {
            send.action = WRONG_CREDENTIALS;
          }
        } else {
          send.action = MAX_ACTIVE_USERS_EXCEEDED;
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
        int i = get_user_position(receive.pid);
        b.board[active_user[i].y][active_user[i].x] = ' ';
        delete_from_active_users_list(receive.pid);
        update_all_users();
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
    case BIGBOMB:
      player_move(BIGBOMB, receive.pid);
      update_all_users();
      break;
    case MINIBOMB:
      player_move(MINIBOMB, receive.pid);
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
  } else if (strcmp(arg[0], "map") == 0) {
    if (arg[1] != NULL) {
      if (load_board(arg[1])) {
        printf("Mapa carregado com sucesso\n");
        map_loaded = true;
      }
    }
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

  signal(SIGINT, SIGhandler);
  signal(SIGUSR1, SIGhandler);

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

  load_game();

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
