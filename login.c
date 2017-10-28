#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct users {
  char username[20];
  char password[20];
  struct users *next;
};

struct users *head = NULL;
struct users *curr = NULL;

struct users *create_list(char username[20], char password[20]) {
  struct users *ptr = (struct users *)malloc(sizeof(struct users));
  if (NULL == ptr) {
    printf("\n Node creation failed \n");
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
    printf("\n Node creation failed \n");
    return NULL;
  }
  strcpy(ptr->username, username);
  strcpy(ptr->password, password);
  ptr->next = NULL;

  curr->next = ptr;
  curr = ptr;

  return ptr;
}

void print_list(void) {
  struct users *ptr = head;
  while (ptr != NULL) {
    printf("Username: [%s]\n", ptr->username);
    printf("Password: [%s] \n\n", ptr->password);
    ptr = ptr->next;
  }

  return;
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

  printf("#### LOGIN ####\n\n");
  printf("\nUsername : ");
  scanf("%s", login.username);
  printf("\nPassword : ");
  scanf("%s", login.password);

  if (validate(login.username, login.password) == 1)
    printf("\n\nLogin efetuado com sucesso\n");
  else
    printf("\n\nAs credênciais estão erradas\n");
}

int main(void) {

  /* Carregar a lista de utilizadores */
  loadusers();

  /* Mostrar a lista de utilizadores */
  print_list();

  /* Efetuar login */
  login();

  return 0;
}
