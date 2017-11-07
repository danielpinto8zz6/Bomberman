#include "database.h"
#include "login.h"
#include "register.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct users *head = NULL;
struct users *curr = NULL;

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
    if (arg[1] != NULL && arg[2] != NULL)
      adduser(arg[1], arg[2]);
    else
      printf("Faltam argumentos\n");
  } else if (strcmp(arg[0], "shutdown") == 0) {
    /* Por enquanto terminamos o programa, mais tarde será implementado
    para
     * terminar o servidor */
    printf("Programa terminado");
    exit(0);
  } else if (strcmp(arg[0], "users") == 0) {
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
