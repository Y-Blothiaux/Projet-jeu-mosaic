#include "game_tools.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

game game_load(char *filename) {
  if (filename == NULL) {
    fprintf(stderr, "NULL arg\n");
    exit(EXIT_FAILURE);
  }
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    fprintf(stderr, "Erreur lors de l'ouverture du fichier\n");
    exit(EXIT_FAILURE);
  }

  int c;
  int pos = 0;
  int count = 0;
  int count2 = 0;
  int rows = 0;
  int col = 0;
  bool wrap = false;
  int neigh_type;
  int i = 0, j = -1;
  // bool isColor;
  // char color;
  // char constraint;
  game g = NULL;
  while ((c = fgetc(file)) != EOF) {
    count++;
    pos++;
    if (count <= 7) {
      if (count == 1) {
        rows = c - '0';
      }
      if (count == 3) {
        col = c - '0';
      }
      if (count == 5) {
        wrap = c - '0';
      }
      if (count == 7) {
        neigh_type = c - '0';
        g = game_new_empty_ext(rows, col, wrap, neigh_type);
      }
    } else {
      if (count2 == 2) {
        i++;
        count2 = 0;
      }
      if (c == 'e' || c == 'E') {
        game_set_color(g, j, i, EMPTY);
        count2++;
      } else if (c == 'w' || c == 'W') {
        game_set_color(g, j, i, WHITE);
        count2++;
      } else if (c == 'b' || c == 'B') {
        game_set_color(g, j, i, BLACK);
        count2++;
      } else if (0 <= c - '0' && c - '0' <= 9) {
        game_set_constraint(g, j, i, c - '0');
        count2++;
      } else if (c == '-') {
        count2++;
      } else if (c == 10) {
        i = 0;
        j++;
        if (j == rows) {
          break;
        }
      } else {
        fprintf(
            stderr,
            "Erreur de charactères dans le fichier save : '%c' avec pos = %d\n",
            c, pos);
        exit(EXIT_FAILURE);
      }
    }
  }

  fclose(file);

  if (g == NULL) {
    fprintf(
        stderr,
        "Erreur lors de la lecture du fichier : fichier vide ou incorrect\n");
    exit(EXIT_FAILURE);
  }

  return g;
}

void game_save(cgame g, char *filename) {
  if (g == NULL || filename == NULL) {
    fprintf(stderr, "NULL arg");
    exit(EXIT_FAILURE);
  }
  FILE *resultat = fopen(filename, "w");
  if (resultat == NULL) {
    fprintf(stderr, "Création du fichier save ratée\n");
    exit(EXIT_FAILURE);
  }
  int nb_cols = game_nb_cols(g);
  int nb_rows = game_nb_rows(g);
  int wapping = game_is_wrapping(g);
  neighbourhood c = game_get_neighbourhood(g);
  int neigh_type = 0;  // De base initialisé a 0 pour FULL
  if (c == ORTHO) {
    neigh_type = 1;
  } else if (c == FULL_EXCLUDE) {
    neigh_type = 2;
  } else if (c == ORTHO_EXCLUDE) {
    neigh_type = 3;
  }
  fprintf(resultat, "%d %d %d %d\n", nb_rows, nb_cols, wapping, neigh_type);
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      char constaint;
      if (game_get_constraint(g, i, j) != UNCONSTRAINED) {
        constaint = game_get_constraint(g, i, j) + '0';
      } else {
        constaint = '-';
      }

      char color = 'e';
      if (game_get_color(g, i, j) == WHITE) {
        color = 'w';
      }
      if (game_get_color(g, i, j) == BLACK) {
        color = 'b';
      }
      fprintf(resultat, "%c%c", constaint, color);
    }
    if (i != nb_rows) {
      fprintf(resultat, "\n");
    }
  }
  fclose(resultat);
}

void genGames(game g, bool version, int pos, int *compteur, int size) {
  // stop recursive calls in case of the end of the "word"
  if (g == NULL || compteur == NULL) {
    fprintf(stderr, "NULL arg");
    exit(EXIT_FAILURE);
  }
  if (pos == size) {
    if (game_won(g) == true) {
      (*compteur)++;
    }
    return;
  }

  // put the value WHITE at position pos of the game color array
  g->colors[pos] = WHITE;
  genGames(g, version, pos + 1, compteur, size);

  // stop recursive calls in case of a call of game_solve, and if a solution has
  // already been found
  if (version == true && *(compteur) > 0) {
    return;
  }

  // put the value BLACK at position pos of the game color array
  g->colors[pos] = BLACK;
  genGames(g, version, pos + 1, compteur, size);
}

bool game_solve(game g) {
  if (g == NULL) {
    fprintf(stderr, "NULL arg");
    exit(EXIT_FAILURE);
  }
  int comp = 0;
  genGames(g, true, 0, &comp, g->nbCols * g->nbRows);
  if (comp == 0) {
    return false;
  }
  return true;
}

uint game_nb_solutions(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "NULL game arg");
    return EXIT_FAILURE;
  }
  int comp = 0;
  game gameCopy = game_copy(g);
  genGames(gameCopy, false, 0, &comp, g->nbCols * g->nbRows);
  game_delete(gameCopy);
  return comp;
}

game game_random(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh,
                 bool with_solution, float black_rate, float constraint_rate) {
  assert(black_rate >= 0.0f && black_rate <= 1.0f);
  assert(constraint_rate >= 0.0f && constraint_rate <= 1.0f);
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  assert(g);

  // fill the grid with random colors
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      color c = (rand() < black_rate * (float)RAND_MAX) ? BLACK : WHITE;
      game_set_color(g, i, j, c);
    }
  }

  // fill the grid with actual constraint at random positions
  uint nb_squares = nb_rows * nb_cols;
  uint nb_constraints = constraint_rate * nb_squares;
  for (uint i = 0; i < nb_constraints; i++) {
    uint row = rand() % nb_rows;
    uint col = rand() % nb_cols;
    int nb_blacks = game_nb_neighbors(g, row, col, BLACK);
    game_set_constraint(g, row, col, nb_blacks);
  }

  // check solution
  if (!game_won(g)) {
    game_delete(g);
    return NULL;
  }

  if (!with_solution) game_restart(g);
  return g;
}