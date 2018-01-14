#include "../structs.h"
#include "enemies.h"
#include "game.h"
#include "main.h"
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void set_enemies() {
  int enemies = get_game_enemies();
  if (enemies > 20)
    enemies = 20;

  int i = get_empty_positions_number();
  coordinates *empty_positions = get_empty_positions(i);
  if (empty_positions) {
    free(empty_positions);
  }

  for (int j = 0; j < enemies; j++) {
    int random = random_number(i);
    int x = empty_positions[random].x;
    int y = empty_positions[random].y;
    b.users[y][x] = '$';
    enemy[j].x = x;
    enemy[j].y = y;
    nr_enemies++;
  }
}

void *thread_enemies(void *arg) {
  for (int i = 0; i < nr_enemies; i++) {
    enemie_move(enemy[i].y, enemy[i].x);
    update_all_users();
    sleep(2);
    if (i == nr_enemies - 1) {
      i = 0;
    }
  }

  return NULL;
}

void enemies() {
  int err;
  pthread_t enemy_thread;

  err = pthread_create(&enemy_thread, NULL, &thread_enemies, NULL);
  if (err != 0)
    printf("Nao foi possivel criar a thread");
}

bool move_enemie(int i, int y, int x) {
  switch (i) {
  case 0:
    if (b.board[y - 1][x] == ' ') {
      if (b.users[y - 1][x] == '*') {
        player_lost(x, y - 1);
        b.users[y - 1][x] = '$';
        b.users[y][x] = ' ';
      } else {
        if (b.users[y - 1][x] != '$') {
          b.users[y - 1][x] = '$';
          b.users[y][x] = ' ';
        } else {
          return false;
        }
      }
      return true;
    } else {
      return false;
    }
    break;
  case 1:
    if (b.board[y][x - 1] == ' ') {
      if (b.users[y][x - 1] == '*') {
        player_lost(x - 1, y);
        b.users[y][x - 1] = '$';
        b.users[y][x - 1] = ' ';
      } else {
        if (b.users[y - 1][x] != '$') {
          b.users[y - 1][x] = '$';
          b.users[y][x] = ' ';
        } else {
          return false;
        }
      }
      return true;
    } else {
      return false;
    }
    break;
  case 2:
    if (b.board[y + 1][x] == ' ') {
      if (b.users[y + 1][x] == '*') {
        player_lost(x, y + 1);
        b.users[y + 1][x] = '$';
        b.users[y][x] = ' ';
      } else {
        if (b.users[y - 1][x] != '$') {
          b.users[y - 1][x] = '$';
          b.users[y][x] = ' ';
        } else {
          return false;
        }
      }
      return true;
    } else {
      return false;
    }
    break;
  case 3:
    if (b.board[y][x + 1] == ' ') {
      if (b.users[y][x + 1] == '*') {
        player_lost(x + 1, y);
        b.users[y][x + 1] = '$';
        b.users[y][x + 1] = ' ';
      } else {
        if (b.users[y - 1][x] != '$') {
          b.users[y - 1][x] = '$';
          b.users[y][x] = ' ';
        } else {
          return false;
        }
      }
      return true;
    } else {
      return false;
    }
    break;
  }
  return false;
}

void enemie_move(int y, int x) {
  int i = random_number(3);
  bool b;

  /*Caso a posição esteja ocupada procurar a primeira livre */
  if (move_enemie(i, y, x) == false) {
    b = move_enemie(0, y, x);
    if (b == false) {
      b = move_enemie(1, y, x);
      if (b == false) {
        b = move_enemie(2, y, x);
        if (b == false) {
          b = move_enemie(3, y, x);
          if (b == false) {
            return;
          }
        } else {
          update_all_users();
        }
      } else {
        update_all_users();
      }
    } else {
      update_all_users();
    }
  } else {
    update_all_users();
  }
}
