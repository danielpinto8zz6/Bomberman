#include "../structs.h"
#include "activeusers.h"
#include "main.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_to_active_users_list(int pid, char username[20]) {
  active_user[nr_active_users].pid = pid;
  strcpy(active_user[nr_active_users].username, username);
  nr_active_users++;
}

int check_if_user_is_logged(char username[20]) {
  for (int i = 0; i < nr_active_users; i++)
    if (strcmp(active_user[i].username, username) == 0)
      return 1;
  return 0;
}

char *get_username_from_pid(int pid) {
  for (int i = 0; i < nr_active_users; i++)
    if (active_user[i].pid == pid)
      return active_user[i].username;
  return NULL;
}

void delete_from_active_users_list(int pid) {
  int i;
  for (i = 0; i < nr_active_users && active_user[i].pid != pid; i++)
    ;
  active_user[i] = active_user[nr_active_users - 1];
  nr_active_users--;
}
void print_active_users_list() {
  for (int i = 0; i < nr_active_users; i++)
    printf("%d -> %s\n", active_user[i].pid, active_user[i].username);
}
