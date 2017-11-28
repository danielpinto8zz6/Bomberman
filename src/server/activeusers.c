#include "../structs.h"
#include "main.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct usersActive *create_active_users_list(int pid, char username[20]) {
  struct usersActive *ptr =
      (struct usersActive *)malloc(sizeof(struct usersActive));
  if (NULL == ptr) {
    printf("Erro ao criar nó \n");
    return NULL;
  }
  ptr->pid = pid;
  strcpy(ptr->username, username);

  active_head = active_curr = ptr;
  return ptr;
}

struct usersActive *add_to_active_users_list(int pid, char username[20]) {
  if (NULL == active_head) {
    return (create_active_users_list(pid, username));
  }

  struct usersActive *ptr =
      (struct usersActive *)malloc(sizeof(struct usersActive));
  if (NULL == ptr) {
    printf("Erro ao criar nó \n");
    return NULL;
  }
  ptr->pid = pid;
  strcpy(ptr->username, username);

  active_curr->next = ptr;
  active_curr = ptr;

  return ptr;
}

struct usersActive *search_in_active_users_list(char username[20],
                                                struct usersActive **prev) {
  struct usersActive *ptr = active_head;
  struct usersActive *tmp = NULL;
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

int delete_from_active_users_list(char username[20]) {
  struct usersActive *prev = NULL;
  struct usersActive *del = NULL;

  del = search_in_active_users_list(username, &prev);
  if (del == NULL) {
    return -1;
  } else {
    if (prev != NULL)
      prev->next = del->next;

    if (del == active_curr) {
      active_curr = prev;
    } else if (del == active_head) {
      active_head = del->next;
    }
  }

  free(del);
  del = NULL;

  return 0;
}

void print_active_users_list() {
  struct usersActive *ptr = active_head;
  while (ptr != NULL) {
    printf("%d -> %s\n", ptr->pid, ptr->username);
    ptr = ptr->next;
  }
}

int check_if_user_is_logged(char username[20]) {
  struct usersActive *active_curr = active_head;
  while (active_curr != NULL) {
    if (strcmp(active_curr->username, username) == 0)
      return 1;
    active_curr = active_curr->next;
  }
  return 0;
}
