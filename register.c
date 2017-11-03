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

void adduser() {
  struct users add;

  printf("#### REGISTER ####\n");
  printf("\nUsername : ");
  scanf("%s", add.username);
  printf("\nPassword : ");
  scanf("%s", add.password);

  /* Verificar que o utilizador nao existe */
  if (check_if_user_exists(add.username) == 0) {
    add_to_list(add.username, add.password);
    save_users_data();
  } else
    printf("\nJá existe um jogador com esse nome registado");
}
