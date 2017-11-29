#include "../structs.h"
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void *receiver(void *arg) {
  char pipe[10];
  int fd_pipe;
  int stop = 0;
  char msg[1024];

  sprintf(pipe, "pipe-%d", getpid());
  mkfifo(pipe, 0600);

  fd_pipe = open(pipe, O_RDWR);

  do {
    read(fd_pipe, &msg, sizeof(msg));
    printf("%s", msg);
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
  usersActive sendData;

  char pipe[10];
  int fd;

  sendData.pid = getpid();
  sendData.action = 2;
  sprintf(pipe, "pipe-%d", getpid());
  fd = open(PIPE, O_WRONLY, 0600);
  write(fd, &sendData, sizeof(sendData));
  unlink(pipe);
  printf("Programa terminado\n");
  exit(0);
}

void SIGhandler(int sig) {
  signal(sig, SIG_IGN);
  shutdown();
}

void error(char *msg) { printf("%s", msg); }

int main(int argc, char *argv[]) {

  usersActive sendCredentials;
  int fd, res;
  char pipe[20];
  pthread_t task;

  signal(SIGINT, SIGhandler);

  setbuf(stdout, NULL);

  if (access(PIPE, F_OK) != 0) {
    error("O servidor não se encontra em execução. A sair...\n");
    exit(0);
  }

  sendCredentials.pid = getpid();
  sprintf(pipe, "pipe-%d", sendCredentials.pid);

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
    printf("#### LOGIN ####\n\n");
    printf("\nUsername : ");
    scanf(" %19[^\n]s", sendCredentials.username);
    printf("\nPassword : ");
    scanf(" %19[^\n]s", sendCredentials.password);
  } while (is_login_fields_valid(sendCredentials) == 0);
  sendCredentials.action = 1;
  write(fd, &sendCredentials, sizeof(sendCredentials));
  sleep(2);

  pthread_join(task, NULL);
  close(fd);
  unlink(pipe);

  return 0;
}
