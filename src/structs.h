#define PIPE "serv_pipe"

#define WIDTH 30
#define HEIGHT 20

#define LOGIN 1
#define LOGOUT 2
#define UP 3
#define DOWN 4
#define LEFT 5
#define RIGHT 6
#define UPDATE 7
#define LOGGED 8
#define WRONG_CREDENTIALS 9
#define ALREADY_LOGGED 10
#define KICK 11
#define SERVER_SHUTDOWN 12
#define MAX_ACTIVE_USERS_EXCEEDED 13
#define MINIBOMB 14
#define BIGBOMB 15

typedef struct {
  char username[20];
  char password[20];
} users;

typedef struct {
  char board[HEIGHT][WIDTH];
  char users[HEIGHT][WIDTH];
  char bombs[HEIGHT][WIDTH];
} Board;

typedef struct {
  char username[20];
  char password[20];
  int pid;
  int action;
  Board board;
  int x;
  int y;
  int pontuation;
  int minibombs;
  int bigbombs;
} usersActive;

typedef struct {
  int x;
  int y;
} coordinates;

typedef struct {
  int x;
  int y;
  int type;
} Bomb;
