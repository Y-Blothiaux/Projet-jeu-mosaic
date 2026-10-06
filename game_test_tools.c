#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"
#include "game_tools.h"

bool test_game_save() {
  game g1 = game_default();
  FILE* f1 = fopen("f1.txt", "w");
  if (f1 == NULL) {
    fprintf(stderr, "can't create a file in test_game_save");
    exit(EXIT_FAILURE);
  }
  game_save(g1, "f1.txt");

  game g2 = game_load("f1.txt");
  bool res = game_equal(g1, g2);
  game_delete(g1);
  game_delete(g2);

  return res;
}

bool test_game_load() {
  game g1;
  game g2 = game_default();

  g1 = game_load("test_save_1.txt");
  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);

  g1 = game_load("test_save_2.txt");
  g2->neighbourhoodType = ORTHO;
  if (game_equal(g1, g2) == false) {
    game_delete(g2);
    return false;
  }
  game_delete(g1);

  g1 = game_load("test_save_3.txt");
  g2->neighbourhoodType = FULL_EXCLUDE;
  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);

  g1 = game_load("test_save_4.txt");
  g2->neighbourhoodType = ORTHO_EXCLUDE;
  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);

  g1 = game_load("test_save_5.txt");
  g2->neighbourhoodType = FULL;
  g2->isWrapping = true;
  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);

  g1 = game_load("test_save_6.txt");
  g2->neighbourhoodType = ORTHO;
  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);

  g1 = game_load("test_save_7.txt");
  g2->neighbourhoodType = FULL_EXCLUDE;
  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);

  g1 = game_load("test_save_8.txt");
  g2->neighbourhoodType = ORTHO_EXCLUDE;
  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);

  game_delete(g2);
  return true;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return EXIT_FAILURE;
  }

  bool ok = false;
  if (strcmp(argv[1], "game_save") == 0) {
    ok = test_game_save();
  }
  if (strcmp(argv[1], "game_load") == 0) {
    ok = test_game_load();
  }
  if (ok == true) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}