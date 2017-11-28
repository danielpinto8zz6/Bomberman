int check_if_user_is_logged(char username[20]);
void print_active_users_list();
int delete_from_active_users_list(char username[20]);
struct usersActive *search_in_active_users_list(char username[20],struct usersActive **prev);
struct usersActive *add_to_active_users_list(int pid,char username[20]);
extern struct usersActive *active_curr;
extern struct usersActive *active_head;
struct usersActive *create_active_users_list(int pid,char username[20]);
