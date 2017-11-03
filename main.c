#include "database.h"
#include "login.h"
#include "register.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct users *head = NULL;
struct users *curr = NULL;

void keyboard(char *cmd) {
  char *arg[3];

  *arg = strtok(cmd, " ");

  if (strcmp(*arg, "add") == 0) {
    int i = 1;
    while (i < 3) {
      *(arg + i) = strtok(NULL, " ");
      i++;
    }
    adduser(*(arg + 1), *(arg + 2));
  }
}

int main(int argc, char *argv[]) {

  char cmd[80];

  /* Carregar utilizadores */
  loadusers();

  // if (argc != 2) {
  //   printf("Numero de argumentos invalido!\n");
  //   exit(3);
  // }

  while (1) {
    scanf(" %79[^\n]", cmd);
    if (cmd[strlen(cmd) - 1] == '\n')
      cmd[strlen(cmd) - 1] = '\0';
    if (strcmp(cmd, " ") != 0)
      keyboard(cmd);
    printf("Comando: %s\n", cmd);
  }
  return 0;
}
