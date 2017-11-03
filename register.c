#include "database.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_if_user_exists(char username[20]) {
  struct users *ptr = head;
  while (ptr != NULL) {
    if (strcmp(ptr->username, username) == 0) {
      return 1;
    }
    ptr = ptr->next;
  }
  return 0;
}

void adduser(char username[20], char password[20]) {

  /* Verificar que o utilizador nao existe */
  if (check_if_user_exists(username) == 0) {
    add_to_list(username, password);
    save_users_data();
  } else
    printf("\nJá existe um jogador com esse nome registado");
}
