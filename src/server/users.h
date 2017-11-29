void add_user(char username[20],char password[20]);
int check_if_user_exists(char username[20]);
void save_users_data();
void loadusers();
void print_users_list();
extern int nr_users;
extern users user[20];
void add_to_users_list(char username[20],char password[20]);
