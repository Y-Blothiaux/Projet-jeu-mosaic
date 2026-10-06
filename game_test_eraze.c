#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"

bool test_game_play_move(void) {
  game g = game_default();

  game_play_move(g, 1, 2, WHITE);
  assert(game_get_color(g, 1, 2) == WHITE);
  assert(game_get_color(g, 2, 1) == EMPTY);

  game_play_move(g, 2, 3, BLACK);
  assert(game_get_color(g, 2, 3) == BLACK);
  assert(game_get_color(g, 3, 2) == EMPTY);

  game_play_move(g, 3, 4, EMPTY);
  assert(game_get_color(g, 3, 4) == EMPTY);

  color avant = game_get_color(g, 3, 1);
  if (avant == WHITE) {
    game_play_move(g, 3, 1, BLACK);
  } else {
    game_play_move(g, 3, 1, WHITE);
  }

  color apres = game_get_color(g, 3, 1);

  if (avant == apres) {
    game_delete(g);
    return false;
  }

  game_delete(g);
  return true;
}

bool test_game_get_next_square(void) {
  game g = game_new_empty();

  unsigned int pi_next = 0;
  unsigned int pj_next = 0;
  if (pi_next || pj_next) {
  }

  assert(game_get_next_square(g, 1, 1, UP, &pi_next, &pj_next) == true);
  assert(game_get_next_square(g, 1, 1, LEFT, &pi_next, &pj_next) == true);
  assert(game_get_next_square(g, 1, 1, RIGHT, &pi_next, &pj_next) == true);
  assert(game_get_next_square(g, 1, 1, DOWN_LEFT, &pi_next, &pj_next) == true);
  assert(game_get_next_square(g, 1, 1, UP_LEFT, &pi_next, &pj_next) == true);
  assert(game_get_next_square(g, 1, 1, DOWN_RIGHT, &pi_next, &pj_next) == true);
  assert(game_get_next_square(g, 1, 1, UP_RIGHT, &pi_next, &pj_next) == true);
  assert(game_get_next_square(g, 1, 1, HERE, &pi_next, &pj_next) == true);

  assert(game_get_next_square(g, 0, 0, UP_RIGHT, &pi_next, &pj_next) == false);
  assert(game_get_next_square(g, 0, 0, UP, &pi_next, &pj_next) == false);
  assert(game_get_next_square(g, 0, 0, LEFT, &pi_next, &pj_next) == false);
  assert(game_get_next_square(g, 0, 0, UP_LEFT, &pi_next, &pj_next) == false);
  assert(game_get_next_square(g, 0, 0, DOWN_LEFT, &pi_next, &pj_next) == false);

  assert(game_get_next_square(g, 4, 4, DOWN_LEFT, &pi_next, &pj_next) == false);
  assert(game_get_next_square(g, 4, 4, DOWN_RIGHT, &pi_next, &pj_next) ==
         false);
  assert(game_get_next_square(g, 4, 4, UP_RIGHT, &pi_next, &pj_next) == false);
  assert(game_get_next_square(g, 4, 4, DOWN, &pi_next, &pj_next) == false);
  assert(game_get_next_square(g, 4, 4, RIGHT, &pi_next, &pj_next) == false);

  if (game_get_next_square(g, 1, 1, DOWN, &pi_next, &pj_next) == true &&
      pi_next == 2 && pj_next == 1) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

bool test_game_set_color(void) {
  game g = game_new_empty();

  game_set_color(g, 1, 1, WHITE);
  assert(game_get_color(g, 1, 1) == WHITE);

  game_set_color(g, 2, 2, BLACK);
  assert(game_get_color(g, 2, 2) == BLACK);

  game_set_color(g, 3, 3, EMPTY);
  assert(game_get_color(g, 3, 3) == EMPTY);

  game_delete(g);
  return true;
}

bool test_game_delete(void) {
  game g = game_new_empty();

  game_delete(g);
  return true;
}

bool test_game_print(void) {
  game g = game_new_empty();

  game_print(g);
  game_delete(g);
  return true;
}

bool test_game_new(void) {
  constraint constraints[25] = {0,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                3,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                5,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                4,
                                UNCONSTRAINED,
                                1,
                                6,
                                UNCONSTRAINED,
                                6,
                                3,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED};
  color colors[25] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY};

  game g = game_new(constraints, colors);
  game g2 = game_default();

  if (game_equal(g, g2)) {
    game_delete(g);
    game_delete(g2);
    return true;
  }

  game_delete(g);
  game_delete(g2);
  return false;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return EXIT_FAILURE;
  }

  bool result = true;

  if (strcmp("game_play_move", argv[1]) == 0) {
    result = test_game_play_move();
  }
  if (strcmp("game_get_next_square", argv[1]) == 0) {
    result = test_game_get_next_square();
  }
  if (strcmp("game_set_color", argv[1]) == 0) {
    result = test_game_set_color();
  }
  if (strcmp("game_delete", argv[1]) == 0) {
    result = test_game_delete();
  }
  if (strcmp("game_print", argv[1]) == 0) {
    result = test_game_print();
  }
  if (strcmp("game_new", argv[1]) == 0) {
    result = test_game_new();
  }

  if (result == true) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
