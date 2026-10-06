#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

game game_new(constraint *constraints, color *colors) {
  if (constraints == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }
  game newGame = game_new_empty();
  if (newGame == NULL || newGame->constraints == NULL) {
    fprintf(stderr, "erreur d'allocation\n");
    game_delete(newGame);
    exit(EXIT_FAILURE);
  }
  for (unsigned i = 0; i < newGame->nbCols * newGame->nbRows; i++) {
    if (colors != NULL) {
      newGame->colors[i] = colors[i];
    }
    newGame->constraints[i] = constraints[i];
  }
  return newGame;
}

game game_new_empty(void) {
  game newGame = malloc(sizeof(struct game_s));
  if (newGame == NULL) {
    fprintf(stderr, "erreur d'allocation\n");
    exit(EXIT_FAILURE);
  }
  constraint *arrayOfConstraint =
      malloc(DEFAULT_SIZE * DEFAULT_SIZE * sizeof(constraint));
  if (arrayOfConstraint == NULL) {
    free(newGame);
    fprintf(stderr, "erreur d'allocation\n");
    exit(EXIT_FAILURE);
  }

  color *arrayOfColor = malloc(DEFAULT_SIZE * DEFAULT_SIZE * sizeof(color));
  if (arrayOfColor == NULL) {
    free(newGame);
    free(arrayOfConstraint);
    fprintf(stderr, "erreur d'allocation\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    arrayOfColor[i] = EMPTY;
    arrayOfConstraint[i] = UNCONSTRAINED;
  }
  newGame->constraints = arrayOfConstraint;
  newGame->colors = arrayOfColor;
  newGame->nbCols = DEFAULT_SIZE;
  newGame->nbRows = DEFAULT_SIZE;
  newGame->isWrapping = false;
  newGame->neighbourhoodType = FULL;
  newGame->redo = queue_new();
  newGame->undo = queue_new();
  if (newGame->undo == NULL || newGame->redo == NULL) {
    fprintf(stderr, "Erreur d'allocation queues\n");
    exit(EXIT_FAILURE);
    game_delete(newGame);
  }
  return newGame;
}

game game_copy(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }
  if (g->constraints == NULL) {
    fprintf(stderr, "Game sans contraintes\n");
    exit(EXIT_FAILURE);
  }
  game g2 = game_new_ext(g->nbRows, g->nbCols, g->constraints, g->colors,
                         g->isWrapping, g->neighbourhoodType);
  if (g2 == NULL) {
    fprintf(stderr, "Pointeur NULL pour la copie\n");
    exit(EXIT_FAILURE);
  }
  return g2;
}

bool game_equal(cgame g1, cgame g2) {
  if (g1 == NULL || g2 == NULL) {
    fprintf(stderr, "arguments invalides\n");
    exit(EXIT_FAILURE);
  }
  if (g1->nbCols != g2->nbCols || g1->nbRows != g2->nbRows ||
      /*g1->isWrapping != g2->isWrapping ||*/
      g1->neighbourhoodType != g2->neighbourhoodType) {
    return false;
  }
  for (int i = 0; i < g1->nbCols; i++) {
    for (int j = 0; j < g1->nbRows; j++) {
      if (game_get_color(g1, i, j) != game_get_color(g2, i, j)) {
        return false;
      }
      if (game_get_constraint(g1, i, j) != game_get_constraint(g2, i, j)) {
        return false;
      }
      if (game_get_status(g1, i, j) != game_get_status(g2, i, j)) {
        return false;
      }
    }
  }
  return true;
}

void game_delete(game g) {
  if (g != NULL) {
    free(g->constraints);
    free(g->colors);
    if (g->undo != NULL) {
      queue_free_full(g->undo, free);
    }
    if (g->redo != NULL) {
      queue_free_full(g->redo, free);
    }
    free(g);
  }
}

void game_set_constraint(game g, uint i, uint j, constraint n) {
  if (g == NULL || i >= g->nbRows || j >= g->nbCols || n < -1 || n > 9) {
    fprintf(stderr, "Paramètres invalides pour game_set_constraint\n");
    exit(EXIT_FAILURE);
  }
  g->constraints[(i * game_nb_cols(g)) + j] = n;
}

color game_get_color(cgame g, uint i, uint j) {
  if (g == NULL) {
    fprintf(stderr, "arguments invalides\n");
    exit(EXIT_FAILURE);
  }
  if (i >= g->nbRows || j >= g->nbCols) {
    fprintf(stderr, "indices 'i' ou 'j' hors limites - get color\n");
    exit(EXIT_FAILURE);
  }
  return g->colors[(i * game_nb_cols(g)) + j];
}

void game_set_color(game g, uint i, uint j, color c) {
  if (g == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }
  if (i >= g->nbRows || j >= g->nbCols) {
    fprintf(stderr, "indices 'i' ou 'j' hors limites - set color\n");
    exit(EXIT_FAILURE);
  }
  if (c != WHITE && c != BLACK && c != EMPTY) {
    fprintf(stderr, "indices 'c' est une mauvaise couleur\n");
    exit(EXIT_FAILURE);
  }
  g->colors[(i * game_nb_cols(g)) + j] = c;
}

constraint game_get_constraint(cgame g, uint i, uint j) {
  if (g == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }
  if (i >= g->nbRows || j >= g->nbCols) {
    fprintf(stderr, "indices 'i' ou 'j' hors limites - get constraint\n");
    exit(EXIT_FAILURE);
  }
  return g->constraints[(i * game_nb_cols(g)) + j];
}

bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next,
                          uint *pj_next) {
  if (g == NULL || pi_next == NULL || pj_next == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }
  if (i >= g->nbRows || j >= g->nbCols) {
    fprintf(stderr, "indices 'i' ou 'j' hors limites - get next square\n");
    exit(EXIT_FAILURE);
  }

  uint nbCols = game_nb_cols(g) - 1;
  uint nbRows = game_nb_rows(g) - 1;

  if (dir == UP && i > 0) {
    *pi_next = i - 1;
    *pj_next = j;
  } else if (dir == DOWN && i < nbRows) {
    *pi_next = i + 1;
    *pj_next = j;
  } else if (dir == LEFT && j > 0) {
    *pi_next = i;
    *pj_next = j - 1;
  } else if (dir == RIGHT && j < nbCols) {
    *pi_next = i;
    *pj_next = j + 1;
  } else if (dir == UP_LEFT && i > 0 && j > 0) {
    *pi_next = i - 1;
    *pj_next = j - 1;
  } else if (dir == UP_RIGHT && i > 0 && j < nbCols) {
    *pi_next = i - 1;
    *pj_next = j + 1;
  } else if (dir == DOWN_LEFT && i < nbRows && j > 0) {
    *pi_next = i + 1;
    *pj_next = j - 1;
  } else if (dir == DOWN_RIGHT && i < nbRows && j < nbCols) {
    *pi_next = i + 1;
    *pj_next = j + 1;
  } else if (dir == HERE) {
    *pi_next = i;
    *pj_next = j;
  } else if (game_is_wrapping(g)) {
    if (dir == UP && i == 0) {
      *pi_next = nbRows;
      *pj_next = j;
    } else if (dir == DOWN && i == nbRows) {
      *pi_next = 0;
      *pj_next = j;
    } else if (dir == LEFT && j == 0) {
      *pi_next = i;
      *pj_next = nbCols;
    } else if (dir == RIGHT && j == nbCols) {
      *pi_next = i;
      *pj_next = 0;
    } else if (dir == UP_LEFT && i == 0 && j > 0) {
      *pi_next = nbRows;
      *pj_next = j - 1;
    } else if (dir == UP_RIGHT && i == 0 && j < nbCols) {
      *pi_next = nbRows;
      *pj_next = j + 1;
    } else if (dir == DOWN_LEFT && i == nbRows && j > 0) {
      *pi_next = 0;
      *pj_next = j - 1;
    } else if (dir == DOWN_RIGHT && i == nbRows && j < nbCols) {
      *pi_next = 0;
      *pj_next = j + 1;
    } else if (dir == UP_LEFT && i > 0 && j == 0) {
      *pi_next = i - 1;
      *pj_next = nbCols;
    } else if (dir == UP_RIGHT && i > 0 && j == nbCols) {
      *pi_next = i - 1;
      *pj_next = 0;
    } else if (dir == DOWN_LEFT && i < nbRows && j == 0) {
      *pi_next = i + 1;
      *pj_next = nbCols;
    } else if (dir == DOWN_RIGHT && i < nbRows && j == nbCols) {
      *pi_next = i + 1;
      *pj_next = 0;
    } else if (dir == UP_RIGHT && i == 0 && j == nbCols) {
      *pi_next = nbRows;
      *pj_next = 0;
    } else if (dir == UP_LEFT && i == 0 && j == 0) {
      *pi_next = nbRows;
      *pj_next = nbCols;
    } else if (dir == DOWN_RIGHT && i == nbRows && j == nbCols) {
      *pi_next = 0;
      *pj_next = 0;
    } else if (dir == DOWN_LEFT && i == nbRows && j == 0) {
      *pi_next = 0;
      *pj_next = nbCols;
    } else {
      return false;
    }
    return true;
  } else {
    return false;
  }
  return true;
}

status game_get_status(cgame g, uint i, uint j) {
  if (g == NULL) {
    fprintf(stderr, "un pointeur en arguments est NULL\n");
    exit(EXIT_FAILURE);
  }
  if (i >= g->nbRows || j >= g->nbCols) {
    fprintf(stderr, "indices 'i' ou 'j' hors limites - get status\n");
    exit(EXIT_FAILURE);
  }
  int valueW = game_nb_neighbors(g, i, j, WHITE);
  int valueB = game_nb_neighbors(g, i, j, BLACK);
  int valueE = game_nb_neighbors(g, i, j, EMPTY);
  constraint comp = game_get_constraint(g, i, j);
  if (comp == -1) {
    if (valueE == 0) {
      return SATISFIED;
    }
    return UNSATISFIED;
  }
  int contrainte_b = (valueB + valueE + valueW) - comp;
  if (valueB > comp || valueW > contrainte_b) {
    return ERROR;
  }
  if (valueB == comp && valueE == 0) {
    return SATISFIED;
  }
  return UNSATISFIED;
}

int game_nb_neighbors(cgame g, uint i, uint j, color c) {
  if (g == NULL) {
    fprintf(stderr, "arguments invalides\n");
    exit(EXIT_FAILURE);
  }
  if (i >= g->nbRows || j >= g->nbCols) {
    fprintf(stderr, "indices 'i' ou 'j' hors limites - nb neighbours\n");
    exit(EXIT_FAILURE);
  }
  if (c != WHITE && c != BLACK && c != EMPTY) {
    fprintf(stderr, "indices 'c' est une mauvaise couleur\n");
    exit(EXIT_FAILURE);
  }
  int count = 0;
  neighbourhood neight_type = game_get_neighbourhood(g);

  uint dir_size;
  direction *directions;
  if (neight_type == FULL) {
    dir_size = 9;
    directions = malloc(dir_size * sizeof(direction));
    for (int i = 0, y = 0; i < dir_size; i++, y++) {
      directions[y] = (direction)i;
    }
  } else if (neight_type == ORTHO) {
    dir_size = 5;
    directions = malloc(dir_size * sizeof(direction));
    for (int i = 0, y = 0; i < 5; i++, y++) {
      directions[y] = (direction)i;
    }
  } else if (neight_type == FULL_EXCLUDE) {
    dir_size = 8;
    directions = malloc(dir_size * sizeof(direction));
    for (int i = 1, y = 0; i < 9; i++, y++) {
      directions[y] = (direction)i;
    }
  } else /*if (neight_type == ORTHO_EXCLUDE) */ {
    dir_size = 4;
    directions = malloc(dir_size * sizeof(direction));
    for (int i = 1, y = 0; i < 5; i++, y++) {
      directions[y] = (direction)i;
    }
  }

  uint ei;
  uint ej;
  for (uint a = 0; a < dir_size; a++) {
    if (game_get_next_square(g, i, j, directions[a], &ei, &ej) == true) {
      if (game_get_color(g, ei, ej) == c) {
        count++;
      }
    }
  }

  free(directions);

  return count;
}

void game_play_move(game g, uint i, uint j, color c) {
  if (g == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }
  if (i >= g->nbRows || j >= g->nbCols) {
    fprintf(stderr, "indices 'i' ou 'j' hors limites - play move\n");
    exit(EXIT_FAILURE);
  }
  if (c != WHITE && c != BLACK && c != EMPTY) {
    fprintf(stderr, "indices 'c' est une mauvaise couleur\n");
    exit(EXIT_FAILURE);
  }
  if (g->redo) {
    queue_clear_full(g->redo, free);
  }
  element *action = malloc(sizeof(element));
  action->i = i;
  action->j = j;
  action->couleur = game_get_color(g, i, j);

  queue_push_tail(g->undo, action);
  game_set_color(g, i, j, c);
}

bool game_won(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_color(g, i, j) == EMPTY) {
        return false;
      }
      if (game_get_constraint(g, i, j) != UNCONSTRAINED &&
          game_get_status(g, i, j) != SATISFIED) {
        return false;
      }
    }
  }
  return true;
}

void game_restart(game g) {
  if (g == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }
  if (g->undo) {
    queue_clear_full(g->undo, free);
  }
  if (g->redo) {
    queue_clear_full(g->redo, free);
  }
  for (int i = 0; i < g->nbRows * g->nbCols; i++) {
    g->colors[i] = EMPTY;
  }
}
