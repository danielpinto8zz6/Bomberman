#include "../structs.h"
#include "activeusers.h"
#include "login.h"
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

int nr_users = 0;
users user[20];

int nr_active_users = 0;
usersActive active_user[20];

void *receiver(void *arg) {
  usersActive receivedCredentials;
  int fd, fd_send;
  bool stop = false;
  char pipe[10], msg[1024];

  mkfifo(PIPE, 0600);
  fd = open(PIPE, O_RDWR);

  do {
    read(fd, &receivedCredentials, sizeof(receivedCredentials));

    sprintf(pipe, "pipe-%d", receivedCredentials.pid);
    fd_send = open(pipe, O_WRONLY, 0600);

    switch (receivedCredentials.action) {
    case 1:
      /* 0 - not logged */
      if (check_if_user_is_logged(receivedCredentials.username) == 0) {
        if (validate(receivedCredentials.username,
                     receivedCredentials.password) == 1) {
          printf("\n%d -> %s iniciou sessão\n", receivedCredentials.pid,
                 receivedCredentials.username);
          strcpy(msg, "Sessão iniciada com sucesso\n");
          add_to_active_users_list(receivedCredentials.pid,
                                   receivedCredentials.username);
        } else {
          sprintf(msg,
                  "\n%d -> %s tentou iniciar sessão, credênciais erradas\n",
                  receivedCredentials.pid, receivedCredentials.username);
          strcpy(msg, "Username ou password errados\n");
        }
      } else {
        strcpy(msg, "O utilizadorja está logado\n");
      }
      write(fd_send, msg, sizeof(msg));
      break;
    case 2:
      if (check_if_user_is_logged(
              get_username_from_pid(receivedCredentials.pid)) == 1) {
        /* User left */
        printf("O cliente %d -> %s saiu do jogo!\n", receivedCredentials.pid,
               get_username_from_pid(receivedCredentials.pid));
        delete_from_active_users_list(receivedCredentials.pid);
      }
    default:
      break;
    }

  } while (stop == false);
  close(fd);
  pthread_exit(0);
}

void shutdown() {
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
