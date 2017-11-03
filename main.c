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
  } else if (strcmp(*arg, "shutdown") == 0) {
    /* Por enquanto terminamos o programa, mais tarde será implementado para
     * terminar o servidor */
    printf("Programa terminado");
    exit(0);
  } else if (strcmp(*arg, "users") == 0) {
    /* Por enquanto mostramos a lista de utilizadores, mais tarde serão
     * mostrados apenas os utilizadores ativos */
    print_list_users();
  } else {
    printf("Comando inválido!\n");
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
    // printf("Comando: %s\n", cmd);
  }
  return 0;
}
