#include "main.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct users *create_list(char username[20], char password[20]) {
  struct users *ptr = (struct users *)malloc(sizeof(struct users));
  if (NULL == ptr) {
    printf("\nFalha ao criar reservar memória");
    return NULL;
  }
  strcpy(ptr->username, username);
  strcpy(ptr->password, password);
  ptr->next = NULL;

  head = curr = ptr;
  return ptr;
}

struct users *add_to_list(char username[20], char password[20]) {
  if (NULL == head) {
    return (create_list(username, password));
  }

  struct users *ptr = (struct users *)malloc(sizeof(struct users));
  if (NULL == ptr) {
    printf("\nFalha ao criar reservar memória");
    return NULL;
  }
  strcpy(ptr->username, username);
  strcpy(ptr->password, password);
  ptr->next = NULL;

  curr->next = ptr;
  curr = ptr;

  return ptr;
}

void save_users_data() {
  struct users *ptr = head;

  FILE *file;
  file = fopen("users_database.txt", "w+");

  if (file != NULL) {

    while (ptr != NULL) {
      fprintf(file, "%s %s\n", ptr->username, ptr->password);
      ptr = ptr->next;
    }
  } else {
    printf("\nNão foi possível conectar com a base de dados");
  }
  fclose(file);
}

void loadusers() {

  struct users load;

  FILE *file;
  file = fopen("users_database.txt", "r");

  if (file != NULL) {
    while (fscanf(file, "%s %s", load.username, load.password) == 2) {
      add_to_list(load.username, load.password);
    }
  } else {
    printf("\nNão foi possível conectar com a base de dados");
  }
  fclose(file);
}

void print_list_users(void) {
  struct users *ptr = head;
  printf("\nLista de users registados\n");
  while (ptr != NULL) {
    printf("\nUsername: [%s]", ptr->username);
    printf("\nPassword: [%s] \n\n", ptr->password);
    ptr = ptr->next;
  }

  return;
}
