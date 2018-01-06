#define PIPE "serv_pipe"

typedef struct {
  char username[20];
  char password[20];
} users;
typedef struct {
  char username[20];
  char password[20];
  int pid;
  int action;
} usersActive;

typedef struct {
  char username[20];
  int pontuation;
  int position_x;
  int position_y;
  int miniBombs;
  int bigBombs;
  int timeToPlay;
} usersPlaying;

typedef struct {
  int block_x[50];
  int block_y[50];
  int exit_x[1];
  int exit_y[1];
  int object_x[50];
  int object_y[50];
  int walls_x[50];
  int walls_y[50];
  int miniBomb_x[50];
  int miniBomb_y[50];
  int bigBomb_x[50];
  int bigBomb_y[50];
  int enemies_x[50];
  int enemies_y[50];
  int colector_x[50];
  int colector_y[50];
  int extra_x[50];
  int extra_y[50];
  int time[1][1][1];
  usersPlaying players;
} Board;

int nr_users;
users user[20];
int nr_active_users;
usersActive active_user[20];
Board board;
