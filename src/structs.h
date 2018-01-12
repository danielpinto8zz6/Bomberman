#define PIPE "serv_pipe"
#define WIDTH 30
#define HEIGHT 20

typedef struct {
  char username[20];
  char password[20];
} users;

typedef struct {
  char board[HEIGHT][WIDTH];
} Board;

typedef struct {
  char username[20];
  char password[20];
  int pid;
  int action;
  Board board;
  int x;
  int y;
} usersActive;

// typedef struct {
//   char username[20];
//   int pontuation;
//   int position_x;
//   int position_y;
//   int miniBombs;
//   int bigBombs;
//   int timeToPlay;
// } usersPlaying;

// typedef struct {
//   int block_x[50];
//   int block_y[50];
//   int exit_x[1];
//   int exit_y[1];
//   int object_x[50];
//   int object_y[50];
//   int walls_x[50];
//   int walls_y[50];
//   int miniBomb_x[50];
//   int miniBomb_y[50];
//   int bigBomb_x[50];
//   int bigBomb_y[50];
//   int enemies_x[50];
//   int enemies_y[50];
//   int colector_x[50];
//   int colector_y[50];
//   int extra_x[50];
//   int extra_y[50];
//   int time[1][1][1];
// } Board;

// Board board;

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
