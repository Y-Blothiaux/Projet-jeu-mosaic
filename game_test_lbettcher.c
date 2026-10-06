#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"

bool test_dummy(void) { return true; }

bool test_game_new_empty() {
  game g = game_new_empty();
  if (g == NULL) {
    return false;
    ;
  }

  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      if (game_get_color(g, x, y) != EMPTY ||
          game_get_constraint(g, x, y) != UNCONSTRAINED) {
        game_delete(g);
        return false;
      }
    }
  }
  if (game_nb_cols(g) != DEFAULT_SIZE || game_nb_rows(g) != DEFAULT_SIZE ||
      game_is_wrapping(g) != false || game_get_neighbourhood(g) != FULL) {
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_default() {
  game g;
  game g2;
  g2 = game_default();
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
  g = game_new(constraints, colors);

  if (game_nb_cols(g2) != DEFAULT_SIZE || game_nb_rows(g2) != DEFAULT_SIZE ||
      game_is_wrapping(g2) != false || game_get_neighbourhood(g2) != FULL) {
    return false;
  }

  if (!game_equal(g, g2)) {
    game_delete(g);
    game_delete(g2);
    return false;
  }
  game_delete(g);
  game_delete(g2);
  return true;
}

bool test_game_get_status() {
  game g = game_default();
  if (game_get_status(g, 0, 0) != UNSATISFIED) {
    game_delete(g);
    return false;
  }
  game_set_color(g, 0, 0, WHITE);
  game_set_color(g, 0, 1, WHITE);
  game_set_color(g, 1, 0, WHITE);
  game_set_color(g, 1, 1, WHITE);
  if (game_get_status(g, 0, 0) != SATISFIED) {
    game_delete(g);
    return false;
  }
  game_set_color(g, 0, 1, BLACK);
  if (game_get_status(g, 0, 0) != ERROR) {
    game_delete(g);
    return false;
  }
  if (game_get_status(g, 3, 3) != UNSATISFIED) {
    game_delete(g);
    return false;
  }
  game_delete(g);

  constraint constraints[25] = {
      UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED,
      UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED,
      UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED,
      UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED,
      UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED,
      UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED, UNCONSTRAINED,
      UNCONSTRAINED};
  color colors[25] = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                      WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                      WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                      WHITE, WHITE, WHITE, WHITE};

  game g2 = game_new_ext(5, 5, constraints, colors, true, FULL);
  game_set_constraint(g2, 0, 0, 3);
  game_set_color(g2, 0, 4, BLACK);
  game_set_color(g2, 0, 1, BLACK);
  game_set_color(g2, 0, 0, BLACK);
  if (game_get_status(g2, 0, 0) != SATISFIED) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  g2 = game_new_ext(5, 5, constraints, colors, true, FULL_EXCLUDE);
  game_set_constraint(g2, 0, 0, 2);
  game_set_constraint(g2, 4, 4, 2);
  game_set_color(g2, 0, 4, BLACK);
  game_set_color(g2, 4, 3, BLACK);
  game_set_color(g2, 0, 1, BLACK);
  if (game_get_status(g2, 0, 0) != SATISFIED ||
      game_get_status(g2, 4, 4) != SATISFIED) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  g2 = game_new_ext(5, 5, constraints, colors, true, ORTHO);
  game_set_constraint(g2, 0, 0, 2);
  game_set_constraint(g2, 4, 4, 2);
  game_set_color(g2, 0, 4, BLACK);
  game_set_color(g2, 4, 4, BLACK);
  game_set_color(g2, 0, 1, BLACK);
  if (game_get_status(g2, 0, 0) != SATISFIED ||
      game_get_status(g2, 4, 4) != SATISFIED) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  g2 = game_new_ext(5, 5, constraints, colors, true, ORTHO_EXCLUDE);
  game_set_constraint(g2, 0, 0, 2);
  game_set_constraint(g2, 4, 4, 2);
  game_set_color(g2, 0, 4, BLACK);
  game_set_color(g2, 4, 3, BLACK);
  game_set_color(g2, 0, 1, BLACK);
  if (game_get_status(g2, 0, 0) != SATISFIED ||
      game_get_status(g2, 4, 4) != SATISFIED) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);
  return true;
}

bool test_game_get_constraint() {
  game g = game_default();
  constraint c1 = 0;
  constraint c2 = 3;
  constraint c3 = UNCONSTRAINED;
  game_set_constraint(g, 3, 3, 6);

  if (game_get_constraint(g, 0, 0) != c1) {
    game_delete(g);
    return false;
  }
  if (game_get_constraint(g, 0, 3) != c2) {
    game_delete(g);
    return false;
  }
  if (game_get_constraint(g, 0, 1) != c3) {
    game_delete(g);
    return false;
  }
  if (game_get_constraint(g, 3, 3) != 6) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_set_constraint() {
  game g = game_new_empty();
  assert(g);
  game_set_constraint(g, 0, 0, 0);
  game_set_constraint(g, 0, 3, 3);
  game_set_constraint(g, 1, 1, 5);
  game_set_constraint(g, 2, 2, 4);
  constraint c1 = game_get_constraint(g, 0, 0);
  constraint c2 = game_get_constraint(g, 0, 3);
  constraint c3 = game_get_constraint(g, 1, 1);
  constraint c4 = game_get_constraint(g, 2, 2);
  constraint c5 = game_get_constraint(g, 2, 4);
  game_set_constraint(g, 2, 4, 1);
  game_set_constraint(g, 3, 0, 6);
  game_set_constraint(g, 3, 2, 6);
  game_set_constraint(g, 3, 3, 3);
  constraint c6 = game_get_constraint(g, 3, 0);
  constraint c7 = game_get_constraint(g, 3, 2);
  constraint c8 = game_get_constraint(g, 3, 3);
  constraint c9 = game_get_constraint(g, 1, 2);

  if (c1 != 0 || c2 != 3 || c3 != 5 || c4 != 4 || c5 != -1 || c6 != 6 ||
      c7 != 6 || c8 != 3 || c9 != -1) {
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_won() {
  game g1 = game_default();
  if (game_won(g1) == true) {
    game_delete(g1);
    return false;
  }
  game_delete(g1);

  game g = game_default_solution();
  if (game_won(g) != true) {
    game_delete(g);
    return false;
  }
  game_delete(g);

  game gg = game_default();
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      game_set_color(gg, x, y, WHITE);
    }
  }
  game_set_color(gg, 1, 1, BLACK);
  game_set_constraint(gg, 1, 1, 6);
  if (game_won(gg) != false) {
    game_delete(gg);
    return false;
  }
  game_delete(gg);

  constraint constraints[25] = {0,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED,
                                UNCONSTRAINED};
  color colors[25] = {EMPTY, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                      WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                      WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                      WHITE, WHITE, WHITE, WHITE};
  game g2 = game_new(constraints, colors);
  game_play_move(g2, 0, 0, WHITE);
  assert(game_won(g2) == true);
  game_delete(g2);

  constraint constraints2[25] = {3,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 UNCONSTRAINED,
                                 2};
  color colors2[25] = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                       WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                       WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                       WHITE, WHITE, WHITE, WHITE};

  g2 = game_new_ext(5, 5, constraints2, colors2, true, FULL);
  game_set_constraint(g2, 0, 0, 3);
  game_set_constraint(g2, 4, 4, 2);
  game_set_color(g2, 0, 4, BLACK);
  game_set_color(g2, 0, 1, BLACK);
  game_set_color(g2, 0, 0, BLACK);
  if (game_won(g2) != true) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  g2 = game_new_ext(5, 5, constraints2, colors2, true, FULL_EXCLUDE);
  game_set_constraint(g2, 0, 0, 2);
  game_set_constraint(g2, 4, 4, 2);
  game_set_color(g2, 0, 4, BLACK);
  game_set_color(g2, 4, 3, BLACK);
  game_set_color(g2, 0, 1, BLACK);
  if (game_won(g2) != true) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  g2 = game_new_ext(5, 5, constraints2, colors2, true, ORTHO);
  game_set_constraint(g2, 0, 0, 2);
  game_set_constraint(g2, 4, 4, 2);
  game_set_color(g2, 0, 4, BLACK);
  game_set_color(g2, 4, 4, BLACK);
  game_set_color(g2, 0, 1, BLACK);
  if (game_won(g2) != true) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  g2 = game_new_ext(5, 5, constraints2, colors2, true, ORTHO_EXCLUDE);
  game_set_constraint(g2, 0, 0, 2);
  game_set_constraint(g2, 4, 4, 2);
  game_set_color(g2, 0, 4, BLACK);
  game_set_color(g2, 4, 3, BLACK);
  game_set_color(g2, 0, 1, BLACK);
  if (game_won(g2) != true) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  return true;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return EXIT_FAILURE;
  }

  bool ok = false;
  if (strcmp(argv[1], "dummy") == 0) {
    ok = test_dummy();
  }
  if (strcmp(argv[1], "game_new_empty") == 0) {
    ok = test_game_new_empty();
  }
  if (strcmp(argv[1], "game_default") == 0) {
    ok = test_game_default();
  }
  if (strcmp(argv[1], "game_get_status") == 0) {
    ok = test_game_get_status();
  }
  if (strcmp(argv[1], "game_get_constraint") == 0) {
    ok = test_game_get_constraint();
  }
  if (strcmp(argv[1], "game_set_constraint") == 0) {
    ok = test_game_set_constraint();
  }
  if (strcmp(argv[1], "game_won") == 0) {
    ok = test_game_won();
  }
  if (ok == true) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
