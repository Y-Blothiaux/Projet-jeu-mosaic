#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

bool test_game_new_ext(void) {
  // Test cases for game_new_ext
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

  game g = game_new_ext(4, 4, constraints, colors, true, FULL);

  if (game_is_wrapping(g) == false) {
    game_delete(g);
    return false;
  }
  if (game_get_neighbourhood(g) != FULL) {
    game_delete(g);
    return false;
  }
  if (game_nb_cols(g) != 4) {
    game_delete(g);
    return false;
  }
  if (game_nb_rows(g) != 4) {
    game_delete(g);
    return false;
  }
  uint k = 0;
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++, k++) {
      if (game_get_constraint(g, i, j) != constraints[k]) {
        game_delete(g);
        return false;
      }
      if (game_get_color(g, i, j) != colors[k]) {
        game_delete(g);
        return false;
      }
    }
  }

  game_delete(g);

  game g1 = game_new_ext(4, 4, constraints, colors, false, ORTHO);

  if (game_is_wrapping(g1) != false) {
    game_delete(g1);
    return false;
  }
  if (game_get_neighbourhood(g1) != ORTHO) {
    game_delete(g1);
    return false;
  }
  if (game_nb_cols(g1) != 4) {
    game_delete(g1);
    return false;
  }
  if (game_nb_rows(g1) != 4) {
    game_delete(g1);
    return false;
  }
  uint k1 = 0;
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++, k1++) {
      if (game_get_constraint(g1, i, j) != constraints[k1]) {
        game_delete(g1);
        return false;
      }
      if (game_get_color(g1, i, j) != colors[k1]) {
        game_delete(g1);
        return false;
      }
    }
  }
  game_delete(g1);
  game g2 = game_new_ext(4, 4, constraints, colors, true, FULL_EXCLUDE);

  if (game_is_wrapping(g2) == false) {
    game_delete(g2);
    return false;
  }
  if (game_get_neighbourhood(g2) != FULL_EXCLUDE) {
    game_delete(g2);
    return false;
  }
  if (game_nb_cols(g2) != 4) {
    game_delete(g2);
    return false;
  }
  if (game_nb_rows(g2) != 4) {
    game_delete(g2);
    return false;
  }
  uint k2 = 0;
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++, k2++) {
      if (game_get_constraint(g2, i, j) != constraints[k2]) {
        game_delete(g2);
        return false;
      }
      if (game_get_color(g2, i, j) != colors[k2]) {
        game_delete(g2);
        return false;
      }
    }
  }
  game_delete(g2);
  game g3 = game_new_ext(4, 4, constraints, colors, false, ORTHO_EXCLUDE);

  if (game_is_wrapping(g3) != false) {
    game_delete(g3);
    return false;
  }
  if (game_get_neighbourhood(g3) != ORTHO_EXCLUDE) {
    game_delete(g3);
    return false;
  }
  if (game_nb_cols(g3) != 4) {
    game_delete(g3);
    return false;
  }
  if (game_nb_rows(g3) != 4) {
    game_delete(g3);
    return false;
  }
  uint k3 = 0;
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++, k3++) {
      if (game_get_constraint(g3, i, j) != constraints[k3]) {
        game_delete(g3);
        return false;
      }
      if (game_get_color(g3, i, j) != colors[k3]) {
        game_delete(g3);
        return false;
      }
    }
  }
  game_delete(g3);
  return true;
}

bool test_game_new_empty_ext(void) {
  // Test cases for game_new_empty_ext
  game g = game_new_empty_ext(4, 4, true, FULL);

  if (game_is_wrapping(g) == false) {
    game_delete(g);
    return false;
  }
  if (game_get_neighbourhood(g) != FULL) {
    game_delete(g);
    return false;
  }
  if (game_nb_cols(g) != 4) {
    game_delete(g);
    return false;
  }
  if (game_nb_rows(g) != 4) {
    game_delete(g);
    return false;
  }
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_constraint(g, i, j) != UNCONSTRAINED) {
        game_delete(g);
        return false;
      }
      if (game_get_color(g, i, j) != EMPTY) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  game g1 = game_new_empty_ext(4, 4, false, ORTHO);

  if (game_is_wrapping(g1) != false) {
    game_delete(g1);
    return false;
  }
  if (game_get_neighbourhood(g1) != ORTHO) {
    game_delete(g1);
    return false;
  }
  if (game_nb_cols(g1) != 4) {
    game_delete(g1);
    return false;
  }
  if (game_nb_rows(g1) != 4) {
    game_delete(g1);
    return false;
  }
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_constraint(g1, i, j) != UNCONSTRAINED) {
        game_delete(g1);
        return false;
      }
      if (game_get_color(g1, i, j) != EMPTY) {
        game_delete(g1);
        return false;
      }
    }
  }
  game_delete(g1);
  game g2 = game_new_empty_ext(4, 4, true, FULL_EXCLUDE);

  if (game_is_wrapping(g2) == false) {
    game_delete(g2);
    return false;
  }
  if (game_get_neighbourhood(g2) != FULL_EXCLUDE) {
    game_delete(g2);
    return false;
  }
  if (game_nb_cols(g2) != 4) {
    game_delete(g2);
    return false;
  }
  if (game_nb_rows(g2) != 4) {
    game_delete(g2);
    return false;
  }
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_constraint(g2, i, j) != UNCONSTRAINED) {
        game_delete(g2);
        return false;
      }
      if (game_get_color(g2, i, j) != EMPTY) {
        game_delete(g2);
        return false;
      }
    }
  }
  game_delete(g2);
  game g3 = game_new_empty_ext(4, 4, false, ORTHO_EXCLUDE);

  if (game_is_wrapping(g3) != false) {
    game_delete(g3);
    return false;
  }
  if (game_get_neighbourhood(g3) != ORTHO_EXCLUDE) {
    game_delete(g3);
    return false;
  }
  if (game_nb_cols(g3) != 4) {
    game_delete(g3);
    return false;
  }
  if (game_nb_rows(g3) != 4) {
    game_delete(g3);
    return false;
  }
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 4; j++) {
      if (game_get_constraint(g3, i, j) != UNCONSTRAINED) {
        game_delete(g3);
        return false;
      }
      if (game_get_color(g3, i, j) != EMPTY) {
        game_delete(g3);
        return false;
      }
    }
  }
  game_delete(g3);
  return true;
}

bool test_game_nb_rows(void) {
  game g2 = game_new_empty_ext(4, 3, true, FULL);
  assert(game_nb_rows(g2) == 4);
  assert(game_nb_rows(g2) != 3);

  game_delete(g2);
  return true;
}

bool test_game_nb_cols(void) {
  game g2 = game_new_empty_ext(4, 3, true, FULL);
  assert(game_nb_cols(g2) == 3);
  assert(game_nb_cols(g2) != 4);

  game_delete(g2);
  return true;
}

bool test_game_is_wrapping(void) {
  // Test cases for game_is_wrapping
  game g = game_default();
  assert(game_is_wrapping(g) ==
         false);  // Assuming wrapping is enabled for default game

  game g2 = game_new_empty_ext(5, 4, true, FULL);
  assert(game_is_wrapping(g2) == true);

  game_delete(g);
  game_delete(g2);
  return true;
}

bool test_game_get_neighbourhood(void) {
  // Test cases for game_get_neighbourhood
  game g = game_default();
  assert(game_get_neighbourhood(g) ==
         FULL);  // Assuming unknown neighborhood for default game

  game g1 = game_new_empty_ext(5, 4, true, FULL);
  assert(game_get_neighbourhood(g1) == FULL);
  game g2 = game_new_empty_ext(5, 4, true, ORTHO);
  assert(game_get_neighbourhood(g2) == ORTHO);
  game g3 = game_new_empty_ext(5, 4, true, FULL_EXCLUDE);
  assert(game_get_neighbourhood(g3) == FULL_EXCLUDE);
  game g4 = game_new_empty_ext(5, 4, true, ORTHO_EXCLUDE);
  assert(game_get_neighbourhood(g4) == ORTHO_EXCLUDE);

  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  return true;
}

bool test_game_undo(void) {
  game g = game_default();

  // Effectuer quelques mouvements pour avoir un historique
  game_play_move(g, 0, 0, BLACK);
  game_play_move(g, 2, 2, WHITE);
  game_play_move(g, 2, 2, BLACK);
  game_play_move(g, 3, 2, BLACK);

  // Annuler un mouvement
  game_undo(g);
  game_undo(g);

  // Vérifier si le mouvement a bien été annulé
  if (game_get_color(g, 2, 2) != WHITE || game_get_color(g, 3, 2) != EMPTY) {
    return false;
  }

  game_undo(g);
  game_undo(g);

  if (game_get_color(g, 2, 2) != EMPTY || game_get_color(g, 0, 0) != EMPTY) {
    return false;
  }

  game_delete(g);
  return true;
}

bool test_game_redo(void) {
  game g = game_default();

  // Effectuer quelques mouvements pour avoir un historique
  game_play_move(g, 0, 0, BLACK);
  game_play_move(g, 1, 1, WHITE);
  game_play_move(g, 2, 2, BLACK);

  // Annuler un mouvement
  game_undo(g);
  game_undo(g);

  // Refaire le mouvement annulé
  game_redo(g);
  game_redo(g);

  // Vérifier si le mouvement a bien été refait
  if (game_get_color(g, 2, 2) != BLACK || game_get_color(g, 1, 1) != WHITE) {
    return false;
  }
  game_undo(g);
  game_undo(g);
  game_undo(g);

  game_redo(g);

  if (game_get_color(g, 0, 0) != BLACK || game_get_color(g, 1, 1) != EMPTY ||
      game_get_color(g, 2, 2) != EMPTY) {
    return false;
  }
  game_delete(g);

  return true;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return EXIT_FAILURE;
  }

  bool result = true;

  if (strcmp("game_new_ext", argv[1]) == 0) {
    result = test_game_new_ext();
  }
  if (strcmp("game_new_empty_ext", argv[1]) == 0) {
    result = test_game_new_empty_ext();
  }
  if (strcmp("game_nb_rows", argv[1]) == 0) {
    result = test_game_nb_rows();
  }
  if (strcmp("game_nb_cols", argv[1]) == 0) {
    result = test_game_nb_cols();
  }
  if (strcmp("game_is_wrapping", argv[1]) == 0) {
    result = test_game_is_wrapping();
  }
  if (strcmp("game_get_neighbourhood", argv[1]) == 0) {
    result = test_game_get_neighbourhood();
  }
  if (strcmp("game_undo", argv[1]) == 0) {
    result = test_game_undo();
  }
  if (strcmp("game_redo", argv[1]) == 0) {
    result = test_game_redo();
  }

  if (result == true) {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
