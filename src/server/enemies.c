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

int get_enemy_position(int y, int x) {
  for (int i = 0; i < nr_enemies; i++)
    if (enemy[i].x == x && enemy[i].y == y)
      return i;
  return -1;
}

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
    b.enemies[y][x] = '$';
    enemy[j].x = x;
    enemy[j].y = y;
    nr_enemies++;
  }
}

void *thread_enemies(void *arg) {
  coordinates *en = (coordinates *)arg;
  while (1) {
    enemy_move(en->y, enemy->x);
    pthread_mutex_lock(&lock);
    update_all_users();
    printf("Move : %d %d\n", enemy->x, enemy->y);
    pthread_mutex_unlock(&lock);
    sleep(5);
  }

  return NULL;
}

void enemies() {
  int err;
  for (int i = 0; i < nr_enemies; i++) {
    err = pthread_create(&enemy_thread[i], NULL, &thread_enemies,
                         (void *)&enemy[i]);
    if (err != 0)
      printf("Nao foi possivel criar a thread");
  }
}

bool move_enemy(int i, int y, int x) {
  int j = get_enemy_position(y, x);
  switch (i) {
  case 0:
    if (b.board[y - 1][x] == ' ' || b.board[y - 1][x] == 'O') {
      if (b.users[y - 1][x] == '*') {
        player_lost(x, y - 1);
        b.enemies[y - 1][x] = '$';
        b.enemies[y][x] = ' ';
        enemy[j].y--;
      } else if (b.enemies[y - 1][x] != '$') {
        b.enemies[y - 1][x] = '$';
        b.enemies[y][x] = ' ';
        enemy[j].y--;
      } else {
        return false;
      }
      return true;
    } else {
      return false;
    }
    break;
  case 1:
    if (b.board[y][x - 1] == ' ' || b.board[y][x - 1] == 'O') {
      if (b.users[y][x - 1] == '*') {
        player_lost(x - 1, y);
        b.enemies[y][x - 1] = '$';
        b.enemies[y][x] = ' ';
        enemy[j].x--;
      } else if (b.enemies[y - 1][x] != '$') {
        b.enemies[y - 1][x] = '$';
        b.enemies[y][x] = ' ';
        enemy[j].x--;
      } else {
        return false;
      }
      return true;
    } else {
      return false;
    }
    break;
  case 2:
    if (b.board[y + 1][x] == ' ' || b.board[y + 1][x] == 'O') {
      if (b.users[y + 1][x] == '*') {
        player_lost(x, y + 1);
        b.enemies[y + 1][x] = '$';
        b.enemies[y][x] = ' ';
        enemy[j].y++;
      } else if (b.enemies[y - 1][x] != '$') {
        b.enemies[y - 1][x] = '$';
        b.enemies[y][x] = ' ';
        enemy[j].y++;
      } else {
        return false;
      }
      return true;
    } else {
      return false;
    }
    break;
  case 3:
    if (b.board[y][x + 1] == ' ' || b.board[y][x + 1] == 'O') {
      if (b.users[y][x + 1] == '*') {
        player_lost(x + 1, y);
        b.enemies[y][x + 1] = '$';
        b.enemies[y][x] = ' ';
        enemy[j].x++;
      } else if (b.enemies[y - 1][x] != '$') {
        b.enemies[y - 1][x] = '$';
        b.enemies[y][x] = ' ';
        enemy[j].x++;
      } else {
        return false;
      }
      return true;
    } else {
      return false;
    }
    break;
  }
  return false;
}

void enemy_move(int y, int x) {
  int i = random_number(3);

  // /*Caso a posição esteja ocupada procurar a primeira livre */
  if (move_enemy(i, y, x) == false)
    if (move_enemy(0, y, x) == false)
      if (move_enemy(1, y, x) == false)
        if (move_enemy(2, y, x) == false)
          if (move_enemy(3, y, x) == false)
            return;
}
