#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"

bool test_dummy(void) { return true; }

bool test_game_copy(void) {
  game g = game_default();
  game g_copy = game_copy(g);
  if (game_equal(g, g_copy) == false) {
    game_delete(g);
    game_delete(g_copy);
    return false;
  }
  game_delete(g);
  game_delete(g_copy);

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
                                UNCONSTRAINED};
  color colors[25] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY};

  game g1 = game_new_ext(5, 5, constraints, colors, true, FULL);
  game g2 = game_copy(g1);

  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);
  game_delete(g2);

  g1 = game_new_ext(5, 5, constraints, colors, false, ORTHO);
  g2 = game_copy(g1);

  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);
  game_delete(g2);

  g1 = game_new_ext(5, 5, constraints, colors, true, FULL_EXCLUDE);
  g2 = game_copy(g1);

  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);
  game_delete(g2);

  g1 = game_new_ext(5, 5, constraints, colors, false, ORTHO_EXCLUDE);
  g2 = game_copy(g1);

  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_equal(void) {
  game game1 = game_default();
  game game2 = game_copy(game1);
  game game3 = game_default();

  game_play_move(game3, 1, 2, WHITE);
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_status(game1, i, j) != game_get_status(game2, i, j)) {
        game_delete(game1);
        game_delete(game2);
        game_delete(game3);
        return false;
      }
      if (game_get_color(game1, i, j) != game_get_color(game2, i, j)) {
        game_delete(game1);
        game_delete(game2);
        game_delete(game3);
        return false;
      }
      if (game_get_constraint(game1, i, j) !=
          game_get_constraint(game2, i, j)) {
        game_delete(game1);
        game_delete(game2);
        game_delete(game3);
        return false;
      }
    }
  }
  if (game_equal(game1, game3) == true) {
    game_delete(game1);
    game_delete(game2);
    game_delete(game3);
    return false;
  }
  game_delete(game1);
  game_delete(game2);
  game_delete(game3);

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
                                UNCONSTRAINED};
  color colors[25] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY};

  game g1 = game_new_ext(5, 5, constraints, colors, true, FULL);
  game g2 = game_new_ext(5, 5, constraints, colors, true, FULL);

  if (game_is_wrapping(g1) != game_is_wrapping(g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  if (game_get_neighbourhood(g1) != game_get_neighbourhood(g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  if (game_nb_cols(g1) != game_nb_cols(g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  if (game_nb_rows(g1) != game_nb_rows(g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  for (uint i = 0; i < 5; i++) {
    for (uint j = 0; j < 5; j++) {
      if (game_get_constraint(g1, i, j) != game_get_constraint(g2, i, j)) {
        game_delete(g1);
        game_delete(g2);
        return false;
      }
      if (game_get_color(g1, i, j) != game_get_color(g2, i, j)) {
        game_delete(g1);
        game_delete(g2);
        return false;
      }
      if (game_get_status(g1, i, j) != game_get_status(g2, i, j)) {
        game_delete(g1);
        game_delete(g2);
        return false;
      }
    }
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_default_solution(void) {
  game perfect = game_default_solution();
  int constraint_size = DEFAULT_SIZE * DEFAULT_SIZE;
  constraint constraints[constraint_size];
  color colors[] = {WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
                    BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE,
                    WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK,
                    BLACK, BLACK, BLACK, WHITE};
  for (int i = 0; i < constraint_size; i++) {
    constraints[i] = UNCONSTRAINED;
  }
  constraints[0] = 0;
  constraints[3] = 3;
  constraints[6] = 5;
  constraints[12] = 4;
  constraints[14] = 1;
  constraints[15] = 6;
  constraints[17] = 6;
  constraints[18] = 3;
  game soluce = game_new(constraints, colors);
  bool result = game_equal(perfect, soluce);
  game_delete(soluce);
  game_delete(perfect);
  return (result);
}

bool test_game_get_color(void) {
  game g = game_default();
  game_set_color(g, 2, 2, WHITE);
  game_set_color(g, 1, 1, BLACK);
  bool result = (game_get_color(g, 2, 2) == WHITE) &&
                (game_get_color(g, 1, 1) == BLACK) &&
                (game_get_color(g, 4, 4) == EMPTY);
  game_delete(g);
  return (result);
}

bool test_game_nb_neighbors(void) {
  game g = game_default_solution();
  bool result = ((game_nb_neighbors(g, 1, 1, WHITE) == 4) &&
                 (game_nb_neighbors(g, 1, 1, BLACK) == 5) &&
                 (game_nb_neighbors(g, 3, 3, EMPTY) == 0));
  game_delete(g);

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

  color colors[25] = {WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
                      BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE,
                      WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK,
                      BLACK, BLACK, BLACK, WHITE};

  game g1 = game_new_ext(5, 5, constraints, colors, true, FULL);
  game g2 = game_new_ext(5, 5, constraints, colors, false, ORTHO);
  game g3 = game_new_ext(5, 5, constraints, colors, true, FULL_EXCLUDE);
  game g4 = game_new_ext(5, 5, constraints, colors, true, ORTHO_EXCLUDE);

  result = ((game_nb_neighbors(g1, 1, 1, WHITE) == 4) &&
            (game_nb_neighbors(g1, 1, 1, BLACK) == 5) &&
            (game_nb_neighbors(g1, 3, 3, EMPTY) == 0));
  if (result == false) {
    game_delete(g1);
    return false;
  }
  game_delete(g1);

  result = ((game_nb_neighbors(g2, 1, 1, WHITE) == 3) &&
            (game_nb_neighbors(g2, 1, 1, BLACK) == 2) &&
            (game_nb_neighbors(g2, 3, 3, EMPTY) == 0));
  if (result == false) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  result = ((game_nb_neighbors(g3, 1, 1, WHITE) == 3) &&
            (game_nb_neighbors(g3, 1, 1, BLACK) == 5) &&
            (game_nb_neighbors(g3, 3, 3, EMPTY) == 0));
  if (result == false) {
    game_delete(g3);
    return false;
  }
  game_delete(g3);

  result = ((game_nb_neighbors(g4, 1, 1, WHITE) == 2) &&
            (game_nb_neighbors(g4, 1, 1, BLACK) == 2) &&
            (game_nb_neighbors(g4, 3, 3, EMPTY) == 0));
  game_delete(g4);

  return (result);
}

bool test_game_restart(void) {
  game g = game_default();
  game g2 = game_default();

  game_play_move(g2, 1, 1, WHITE);
  game_play_move(g2, 1, 2, BLACK);
  game_restart(g2);

  bool result = game_equal(g2, g);
  game_delete(g);
  game_delete(g2);
  return result;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0) ok = test_dummy();
  if (strcmp("game_copy", argv[1]) == 0) ok = test_game_copy();
  if (strcmp("game_equal", argv[1]) == 0) ok = test_game_equal();
  if (strcmp("game_default_solution", argv[1]) == 0)
    ok = test_game_default_solution();
  if (strcmp("game_get_color", argv[1]) == 0) ok = test_game_get_color();
  if (strcmp("game_nb_neighbors", argv[1]) == 0) ok = test_game_nb_neighbors();
  if (strcmp("game_restart", argv[1]) == 0) ok = test_game_restart();

  if (ok == true) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}