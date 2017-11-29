#include "../structs.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_to_users_list(char username[20], char password[20]) {
  strcpy(user[nr_users].username, username);
  strcpy(user[nr_users].password, password);
  nr_users++;
}

void print_users_list() {
  for (int i = 0; i < nr_users; i++)
    printf("%s\n", user[i].username);
}

void loadusers() {
  users load;

  FILE *file;
  file = fopen("users_database.txt", "r");

  if (file != NULL) {
    while (fscanf(file, "%s %s", load.username, load.password) == 2) {
      add_to_users_list(load.username, load.password);
    }
  } else {
    printf("\nNão foi possível conectar com a base de dados");
  }
  fclose(file);
}

void save_users_data() {
  FILE *file;
  file = fopen("users_database.txt", "w+");

  if (file != NULL) {

    for (int i = 0; i < nr_users; i++)
      fprintf(file, "%s %s\n", user[i].username, user[i].password);
  } else {
    printf("\nNão foi possível conectar com a base de dados");
  }
  fclose(file);
}

int check_if_user_exists(char username[20]) {
  for (int i = 0; i < nr_users; i++)
    if (strcmp(user[i].username, username) == 0)
      return 1;
  return 0;
}

void add_user(char username[20], char password[20]) {
  /* Verificar que o utilizador nao existe */
  if (check_if_user_exists(username) == 0) {
    add_to_users_list(username, password);
    save_users_data();
    printf("Utilizador adicionado com sucesso\n");
  } else
    printf("\nJá existe um jogador com esse nome registado\n");
}
