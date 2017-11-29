#include "../structs.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validate(char username[20], char password[20]) {
  for (int i = 0; i < nr_users; i++) {
    if (strcmp(user[i].username, username) == 0) {
      if (strcmp(user[i].password, password) == 0) {
        return 1;
      }
    }
  }
  return 0;
}

void login() {
  users login;

  printf("#### LOGIN ####\n");
  printf("\nUsername : ");
  scanf("%19[^\n]s", login.username);
  printf("\nPassword : ");
  scanf("%19[^\n]s", login.password);

  if (validate(login.username, login.password) == 1)
    printf("\nLogin efetuado com sucesso\n");
  else
    printf("\nAs credênciais estão erradas\n");
}
