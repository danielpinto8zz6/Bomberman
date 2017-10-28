void print_list_users(void);
void loadusers();
void save_users_data();
struct users *add_to_list(char username[20],char password[20]);
extern struct users *curr;
extern struct users *head;
struct users *create_list(char username[20],char password[20]);
