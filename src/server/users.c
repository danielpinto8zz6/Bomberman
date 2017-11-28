#include "../structs.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct users *create_users_list(char username[20], char password[20]) {
  struct users *ptr = (struct users *)malloc(sizeof(struct users));
  if (NULL == ptr) {
    printf("\n Node creation failed \n");
    return NULL;
  }
  strcpy(ptr->username, username);
  strcpy(ptr->password, password);

  head = curr = ptr;
  return ptr;
}

struct users *add_to_users_list(char username[20], char password[20]) {
  if (NULL == head) {
    return (create_users_list(username, password));
  }

  struct users *ptr = (struct users *)malloc(sizeof(struct users));
  if (NULL == ptr) {
    printf("Erro ao criar nó \n");
    return NULL;
  }
  strcpy(ptr->username, username);
  strcpy(ptr->password, password);

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

struct users *search_in_users_list(char username[20], struct users **prev) {
  struct users *ptr = head;
  struct users *tmp = NULL;
  int found = 0;

  while (ptr != NULL) {
    if (strcmp(ptr->username, username)) {
      found = 1;
      break;
    } else {
      tmp = ptr;
      ptr = ptr->next;
    }
  }

  if (found == 1) {
    if (prev)
      *prev = tmp;
    return ptr;
  } else {
    return NULL;
  }
}

void adduser(char username[20], char password[20]) {
  /* Verificar que o utilizador nao existe */
  if (search_in_users_list(username, NULL) == 0) {
    add_to_users_list(username, password);
    save_users_data();
    printf("Utilizador adicionado com sucesso\n");
  } else
    printf("\nJá existe um jogador com esse nome registado\n");
}

void loadusers() {

  struct users load;

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

void print_users_list() {
  struct users *ptr = head;
  while (ptr != NULL) {
    printf("%s\n", ptr->username);
    ptr = ptr->next;
  }
}
