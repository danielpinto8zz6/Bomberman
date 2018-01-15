#include "../structs.h"
#include "bombs.h"
#include "enemies.h"
#include "game.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

void check_game_state() {
  if (game_exit == 0) {
    if (get_nr_objects() == 0) {
      printf("Todos os objetos foram apanhados\n");
      game_exit = 1;
      coordinates c = get_first_empty_position_found();
      b.board[c.y][c.x] = 'S';
    }
  }
}

void start_players_positions() {
  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
      b.users[y][x] = ' ';
}

coordinates get_first_empty_position_found() {
  coordinates pos;
  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
      if (b.board[y][x] == ' ' && b.users[y][x] == ' ') {
        pos.y = y;
        pos.x = x;
        break;
      }
  return pos;
}

int get_nr_objects() {
  int objects = 0;
  for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
      if (b.board[y][x] == 'O')
        objects++;
  return objects;
}

void show_game_info() {
  for (int i = 0; i < nr_active_users; i++) {
    if (active_user[i].playing == PLAYING) {
      printf("%s - [%d, %d]\n", active_user[i].username, active_user[i].x,
             active_user[i].y);
      printf("\tBombinhas : %d\n", active_user[i].minibombs);
      printf("\tMega Bombas : %d\n", active_user[i].minibombs);
      printf("\tPontuacao : %d\n", active_user[i].pontuation);
    }
  }
  int objects = get_nr_objects();
  printf("\nObjetos por apanhar : %d\n", objects);
  printf("\nInimigos: %d\n", nr_enemies);
  for (int i = 0; i < nr_enemies; i++) {
    printf("%d - [%d, %d]\n", i, enemy[i].x, enemy[i].y);
  }
}

void player_lost(int x, int y) {
  for (int i = 0; i < nr_active_users; i++) {
    if (active_user[i].x == x && active_user[i].y == y) {
      active_user[i].playing = LOST;
      b.users[y][x] = ' ';
      send_update(active_user[i].pid);
    }
  }
}

void enemy_lost(int x, int y) {
  int i;
  for (i = 0; i < nr_enemies && enemy[i].x != x && enemy[i].y != y; i++)
    ;
  enemy[i] = enemy[nr_enemies - 1];
  nr_enemies--;
}

int get_game_enemies() {
  int enermies = 0;
  const char *env = getenv("NENEMY");
  if (env != NULL) {
    enermies = atoi(env) + game;
  } else {
    enermies = random_number(20) + game;
  }
  return enermies;
}

int get_game_objects() {
  int objects = 0;
  const char *env = getenv("NOBJECT");
  if (env != NULL) {
    objects = atoi(env) + game;
  } else {
    objects = random_number(50) + game;
  }
  return objects;
}

void load_game() {
  start_players_positions();
  start_bombs_positions();
  fill_board();
  place_objects();
  set_enemies();
  // enemies();
}

bool load_board(char *filename) {
  FILE *f;
  char c;
  int y = 0, x = 0;

  f = fopen(filename, "r");

  if (!f) {
    return false;
  } else {
    while ((c = fgetc(f)) != EOF) {

      if (c == '\n') {
        y++;
        x = 0;
      } else {
        b.board[y][x] = c;
        x++;
      }
    }
  }
  return true;
}

int get_empty_positions_number() {
  int i = 0;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (b.board[y][x] == ' ') {
        i++;
      }
    }
  }
  return i;
}

void *get_empty_positions(int count) {
  coordinates *empty = NULL;
  empty = malloc(sizeof(empty) * count);

  int i = 0;

  if (!empty)
    return NULL;

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (b.board[y][x] == ' ') {
        empty[i].x = x;
        empty[i].y = y;
        i++;
      }
    }
  }

  return empty;
}

void place_objects() {
  int objects = get_game_objects();

  int x = 0;
  int y = 0;
  int random = 0;

  int i = get_empty_positions_number();
  coordinates *empty_positions = get_empty_positions(i);
  if (empty_positions) {
    free(empty_positions);
  }

  for (int j = 0; j < objects; j++) {
    random = random_number(i);
    x = empty_positions[random].x;
    y = empty_positions[random].y;
    b.board[y][x] = 'O';
  }
}

void fill_board() {
  if (map_loaded == true) {
    return;
  } else {
    char board[HEIGHT][WIDTH] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '&', '&', '&', '&', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '&', '&', '&', '&', '&', ' ',
        ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#',
        '#', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', '&', '&', ' ', ' ', ' ', ' ',
        '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
        ' ', ' ', '&', '&', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '&', '&', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
        ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        '#', '#', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    };

    for (int y = 0; y < HEIGHT; y++)
      for (int x = 0; x < WIDTH; x++)
        b.board[y][x] = board[y][x];
  }
}

void set_player_position(int pid, int x, int y) {
  for (int i = 0; i < nr_active_users; i++) {
    if (active_user[i].pid == pid) {
      active_user[i].x = x;
      active_user[i].y = y;
      b.users[y][x] = '*';
    }
  }
}

coordinates get_player_position(int pid) {
  coordinates pos;
  for (int i = 0; i < nr_active_users; i++) {
    if (active_user[i].pid == pid) {
      pos.x = active_user[i].x;
      pos.y = active_user[i].y;
    }
  }
  return pos;
}

int get_user_position(int pid) {
  int i = 0;
  for (i = 0; i < nr_active_users; i++) {
    if (pid == active_user[i].pid) {
      return i;
    }
  }
  return -1;
}

bool check_occupied(int pos_x, int pos_y) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (pos_x == x && pos_y == y) {
        if (b.board[y][x] == '#' || b.board[y][x] == '&' ||
            b.users[y][x] == '*') {
          return true;
        }
      }
    }
  }
  return false;
}

void player_move(int move, int pid) {
  int i = get_user_position(pid);
  int x = active_user[i].x;
  int y = active_user[i].y;
  Bomb bomb;

  switch (move) {
  case UP:
    /* y-- */
    y--;
    if (y >= 0 && check_occupied(x, y) == false) {
      if (b.enemies[y][x] == '$') {
        player_lost(x, active_user[i].y);
      } else {
        if (b.board[y][x] == 'O') {
          active_user[i].pontuation++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'B') {
          active_user[i].bigbombs++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'b') {
          active_user[i].minibombs++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'E') {
          active_user[i].pontuation = active_user[i].pontuation + 10;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'C') {
          int colector = 0;
          for (int by = 0; by < HEIGHT; by++)
            for (int bx = 0; bx < WIDTH; bx++)
              if (b.board[by][bx] == 'O' && colector < 5) {
                b.board[by][bx] = ' ';
                active_user[i].pontuation++;
                colector++;
              }
          b.board[y][x] = ' ';
        }
        b.users[y][x] = '*';
        b.users[active_user[i].y][x] = ' ';
        active_user[i].y = y;
      }
    }
    break;
  case DOWN:
    /* y++ */
    y++;
    if (y < HEIGHT && check_occupied(x, y) == false) {
      if (b.enemies[y][x] == '$') {
        player_lost(x, active_user[i].y);
      } else {
        if (b.board[y][x] == 'O') {
          active_user[i].pontuation++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'B') {
          active_user[i].bigbombs++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'b') {
          active_user[i].minibombs++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'E') {
          active_user[i].pontuation = active_user[i].pontuation + 10;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'C') {
          int colector = 0;
          for (int by = 0; by < HEIGHT; by++)
            for (int bx = 0; bx < WIDTH; bx++)
              if (b.board[by][bx] == 'O' && colector < 5) {
                b.board[by][bx] = ' ';
                active_user[i].pontuation++;
                colector++;
              }
          b.board[y][x] = ' ';
        }
        b.users[y][x] = '*';
        b.users[active_user[i].y][x] = ' ';
        active_user[i].y = y;
      }
    }
    break;
  case LEFT:
    /* x-- */
    x--;
    if (x >= 0 && check_occupied(x, y) == false) {
      if (b.enemies[y][x] == '$') {
        player_lost(active_user[i].x, y);
      } else {
        if (b.board[y][x] == 'O') {
          active_user[i].pontuation++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'B') {
          active_user[i].bigbombs++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'b') {
          active_user[i].minibombs++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'E') {
          active_user[i].pontuation = active_user[i].pontuation + 10;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'C') {
          int colector = 0;
          for (int by = 0; by < HEIGHT; by++)
            for (int bx = 0; bx < WIDTH; bx++)
              if (b.board[by][bx] == 'O' && colector < 5) {
                b.board[by][bx] = ' ';
                active_user[i].pontuation++;
                colector++;
              }
          b.board[y][x] = ' ';
        }
        b.users[y][x] = '*';
        b.users[y][active_user[i].x] = ' ';
        active_user[i].x = x;
      }
    }
    break;
  case RIGHT:
    /* x++ */
    x++;
    if (x < WIDTH && check_occupied(x, y) == false) {
      if (b.enemies[y][x] == '$') {
        player_lost(active_user[i].x, y);
      } else {
        if (b.board[y][x] == 'O') {
          active_user[i].pontuation++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'B') {
          active_user[i].bigbombs++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'b') {
          active_user[i].minibombs++;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'E') {
          active_user[i].pontuation = active_user[i].pontuation + 10;
          b.board[y][x] = ' ';
        } else if (b.board[y][x] == 'C') {
          int colector = 0;
          for (int by = 0; by < HEIGHT; by++)
            for (int bx = 0; bx < WIDTH; bx++)
              if (b.board[by][bx] == 'O' && colector < 5) {
                b.board[by][bx] = ' ';
                active_user[i].pontuation++;
                colector++;
              }
          b.board[y][x] = ' ';
        }
        b.users[y][x] = '*';
        b.users[y][active_user[i].x] = ' ';
        active_user[i].x = x;
      }
    }
    break;
  case BIGBOMB:
    if (active_user[i].bigbombs > 0) {
      /*You can't place a bomb on top of another */
      if (b.bombs[y][x] != 'X' && b.bombs[y][x] != 'x') {
        active_user[i].bigbombs--;
        b.bombs[y][x] = 'X';
        bomb.x = x;
        bomb.y = y;
        bomb.type = BIGBOMB;
        place_bomb(&bomb);
      }
    }
    break;
  case MINIBOMB:
    if (active_user[i].minibombs > 0) {
      if (b.bombs[y][x] != 'X' && b.bombs[y][x] != 'x') {
        active_user[i].minibombs--;
        b.bombs[y][x] = 'x';
        bomb.x = x;
        bomb.y = y;
        bomb.type = MINIBOMB;
        place_bomb(&bomb);
      }
    }
    break;
  }
}
