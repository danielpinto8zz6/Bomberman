#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"
#include "database.h"
#include "register.h"

struct users *head = NULL;
struct users *curr = NULL;

int main(void) {

  /* Carregar a lista de utilizadores */
  loadusers();

  /* Mostrar a lista de utilizadores */
  print_list_users();

  /* Efetuar Registo */
  adduser();

  /* Efetuar Login */
  login();

  /* Guardar dados no ficheiro de texto */
  save_users_data();

  return 0;
}
