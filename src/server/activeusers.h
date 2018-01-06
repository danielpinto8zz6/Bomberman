void print_active_users_list();
void delete_from_active_users_list(int pid);
char *get_username_from_pid(int pid);
int check_if_user_is_logged(char username[20]);
void add_to_active_users_list(int pid,char username[20]);
