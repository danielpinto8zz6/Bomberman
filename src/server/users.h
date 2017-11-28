void print_users_list();
void loadusers();
void adduser(char username[20],char password[20]);
struct users *search_in_users_list(char username[20],struct users **prev);
void save_users_data();
struct users *add_to_users_list(char username[20],char password[20]);
extern struct users *curr;
extern struct users *head;
struct users *create_users_list(char username[20],char password[20]);
