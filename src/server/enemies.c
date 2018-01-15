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

bool check_if_enemy_exists(int y, int x) {
  for (int i = 0; i < nr_active_users; i++) {
    if (enemy[i].y == y && enemy[i].x == x)
      return true;
  }
  return false;
}

void *thread_enemies(void *arg) {
  int i = 0;
  while (i < nr_enemies) {
    if (check_if_enemy_exists(enemy[i].y, enemy[i].x) == true) {
      enemy_move(enemy[i].y, enemy[i].x);
      pthread_mutex_lock(&lock);
      update_all_users();
      pthread_mutex_unlock(&lock);
      sleep(2);
    }
    if (i == nr_enemies - 1) {
      i = 0;
    } else {
      i++;
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

bool move_enemy(int i, int y, int x) {

  int j = get_enemy_position(y, x);
  int start_x = x, start_y = y;

  switch (i) {
  case 0:
    x--;
    break;
  case 1:
    x++;
    break;
  case 2:
    y--;
    break;
  case 3:
    y++;
    break;
  }

  if (b.board[y][x] == ' ' || b.board[y][x] == 'O') {
    if (b.users[y][x] == '*') {
      player_lost(x, y);
      b.enemies[y][x] = '$';
      b.enemies[start_y][start_x] = ' ';
      switch (i) {
      case 0:
        enemy[j].x--;
        break;
      case 1:
        enemy[j].x++;
        break;
      case 2:
        enemy[j].y--;
        break;
      case 3:
        enemy[j].y++;
        break;
      }
      return true;
    } else if (b.enemies[y - 1][x] != '$') {
      b.enemies[y][x] = '$';
      b.enemies[start_y][start_x] = ' ';
      switch (i) {
      case 0:
        enemy[j].x--;
        break;
      case 1:
        enemy[j].x++;
        break;
      case 2:
        enemy[j].y--;
        break;
      case 3:
        enemy[j].y++;
        break;
      }
      return true;
    }
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
