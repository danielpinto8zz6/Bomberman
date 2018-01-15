#include "../structs.h"
#include "bombs.h"
#include "game.h"
#include "main.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int bomb_x, bomb_y, bomb_type;

void start_bombs_positions() {
  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
      b.bombs[y][x] = ' ';
}

void *thread_bomb(void *arg) {
  // Bomb *bomb = (Bomb *)arg;
  int bx = bomb_x;
  int by = bomb_y;
  int type = bomb_type;

  sleep(2);

  if (type == MINIBOMB) {
    for (int y = by - 2; y <= by + 2; y++)
      for (int x = bx - 2; x <= bx + 2; x++)
        if (b.board[y][x] != '#')
          b.bombs[y][x] = 'x';
    pthread_mutex_lock(&lock);
    update_all_users();
    pthread_mutex_unlock(&lock);
    sleep(2);
    for (int y = by - 2; y <= by + 2; y++)
      for (int x = bx - 2; x <= bx + 2; x++)
        if (b.board[y][x] != '#') {
          b.bombs[y][x] = ' ';
          b.board[y][x] = ' ';
          if (b.users[y][x] == '*') {
            player_lost(x, y);
          } else if (b.enemies[y][x] == '$') {
            b.enemies[y][x] = ' ';
            enemy_lost(x, y);
          }
        }
    pthread_mutex_lock(&lock);
    update_all_users();
    pthread_mutex_unlock(&lock);
  } else if (type == BIGBOMB) {
    for (int y = by - 4; y <= by + 4; y++)
      for (int x = bx - 4; x <= bx + 4; x++)
        if (b.board[y][x] != '#')
          b.bombs[y][x] = 'X';
    pthread_mutex_lock(&lock);
    update_all_users();
    pthread_mutex_unlock(&lock);
    sleep(2);
    for (int y = by - 4; y <= by + 4; y++)
      for (int x = bx - 4; x <= bx + 4; x++)
        if (b.board[y][x] != '#') {
          b.bombs[y][x] = ' ';
          b.board[y][x] = ' ';
          if (b.users[y][x] == '*') {
            player_lost(x, y);
          } else if (b.enemies[y][x] == '$') {
            b.enemies[y][x] = ' ';
            enemy_lost(x, y);
          }
        }
    pthread_mutex_lock(&lock);
    update_all_users();
    pthread_mutex_unlock(&lock);
  }
  pthread_exit(0);
}

void place_bomb(Bomb *bomb) {
  /* Vou usar variáveis globais visto que ao passar a estrutura com parametro
   * está a perder dados */
  bomb_x = bomb->x;
  bomb_y = bomb->y;
  bomb_type = bomb->type;

  int res = pthread_create(&thread_bombs, NULL, thread_bomb, (void *)(bomb));

  if (res != 0) {
    perror("ERRO! A criar a thread!!!\n");
  }
}
