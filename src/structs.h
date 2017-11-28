#define PIPE "serv_pipe"

struct users {
  char username[20];
  char password[20];
  struct users *next;
};
struct usersActive {
  char username[20];
  char password[20];
  int pid;
  struct usersActive *next;
};

struct usersPlaying {
  char username[20];
  int pontuation;
  int position[1][1];
  int miniBombs;
  int bigBombs;
  int timeToPlay;
};

struct board {
  int block[50][50];
  int exit[1][1];
  int object[50][50];
  int walls[50][50];
  int miniBomb[50][50];
  int bigBomb[50][50];
  int enemies[50][50];
  int colector[50][50];
  int extra[50][50];
  int time[1][1][1];
  struct usersPlaying players;
};
