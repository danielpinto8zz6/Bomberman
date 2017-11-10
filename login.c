#include "main.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validate(char username[20], char password[20]) {
  struct users *ptr = head;
  while (ptr != NULL) {
    if (strcmp(ptr->username, username) == 0) {
      if (strcmp(ptr->password, password) == 0) {
        return 1;
      }
    }
    ptr = ptr->next;
  }
  return 0;
}

void login() {
  struct users login;

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
