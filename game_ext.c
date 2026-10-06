#include "game_ext.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"
#include "queue.c"

game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints,
                  color *colors, bool wrapping, neighbourhood neigh) {
  /*
  Creates a new game with all the new specifications of the 2th version,
  dynamically allocate, pointed by a "newGame" variable. Give this pointer the
  size, wrapping and neighbourhood put in parameter.  Initializes tiles with
  arrays arguments "constraints" and "colors" Only initialize tiles with "EMPTY"
  if colors array is NULL
  This fonction use game_new_empty_ext() (read it for more infos)
  */

  if (constraints == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }

  game newGame = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  uint k = 0;
  if (colors != NULL) {
    for (uint i = 0; i < nb_rows; i++) {
      for (uint j = 0; j < nb_cols; j++, k++) {
        newGame->colors[k] = colors[k];
        newGame->constraints[k] = constraints[k];
      }
    }
  } else {
    for (uint i = 0; i < nb_rows; i++) {
      for (uint j = 0; j < nb_cols; j++, k++) {
        newGame->constraints[k] = constraints[k];
      }
    }
  }

  return newGame;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        neighbourhood neigh) {
  /*
  Creates a new game, dynamically allocate, pointed by a "newGame" variable
  Also allocate 2 arrays member of the struct, temporarily pointed in
  arrayOfConstraint and arrayOfColors var Give it the size define with nb_rows
  and nb_cols, wrapping and neighbourhood. Initializes every tiles with EMPTY
  colors and UNCONSTRAINT constraint This function use queue_new() of queue lib,
  to initialize undo and redo member
  */
  game newGame = malloc(sizeof(struct game_s));
  if (newGame == NULL) {
    fprintf(stderr, "erreur d'allocation\n");
    exit(EXIT_FAILURE);
  }

  constraint *Constraints = malloc(nb_rows * nb_cols * sizeof(constraint));
  if (Constraints == NULL) {
    free(newGame);
    fprintf(stderr, "erreur d'allocation\n");
    exit(EXIT_FAILURE);
  }

  color *Colors = malloc(nb_rows * nb_cols * sizeof(color));
  if (Colors == NULL) {
    free(newGame);
    free(Constraints);
    fprintf(stderr, "erreur d'allocation\n");
    exit(EXIT_FAILURE);
  }

  for (unsigned i = 0; i < nb_rows * nb_cols; i++) {
    Colors[i] = EMPTY;
    Constraints[i] = UNCONSTRAINED;
  }
  newGame->constraints = Constraints;
  newGame->colors = Colors;
  newGame->nbCols = nb_cols;
  newGame->nbRows = nb_rows;
  newGame->isWrapping = wrapping;
  newGame->neighbourhoodType = neigh;
  newGame->redo = queue_new();
  newGame->undo = queue_new();
  if (newGame->undo == NULL || newGame->redo == NULL) {
    fprintf(stderr, "Erreur d'allocation queues\n");
    exit(EXIT_FAILURE);
    game_delete(newGame);
  }
  return newGame;
}

uint game_nb_rows(cgame g) {
  /*
  Return number of rows of a game g
  */
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  return g->nbRows;
}

uint game_nb_cols(cgame g) {
  /*
  Return number of columns of a game g
  */
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  return g->nbCols;
}

bool game_is_wrapping(cgame g) {
  /*
  Return the wrapping of the game g
  */
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  return g->isWrapping;
}

neighbourhood game_get_neighbourhood(cgame g) {
  /*
  Return the neighbourhood type of a game
  */
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }
  return g->neighbourhoodType;
}

void game_undo(game g) {
  /*
  Undo the last move played.
  */
  if (g == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }

  if (!queue_is_empty(g->undo)) {
    element *action = (element *)queue_peek_tail(g->undo);
    queue_pop_tail(g->undo);

    color couleur_transfer = game_get_color(g, action->i, action->j);
    element *replace = malloc(sizeof(element));
    replace->i = action->i;
    replace->j = action->j;
    replace->couleur = couleur_transfer;
    queue_push_tail(g->redo, replace);

    game_set_color(g, action->i, action->j, action->couleur);
    free(action);
    //  Pas besoin de libérer la mémoire car on manipule les éléments dans la
    //  file
  }
}

void game_redo(game g) {
  /*
  Redoes thes last moves, which has been undone by undo.
  */
  if (g == NULL) {
    fprintf(stderr, "Pointeur NULL passé en argument\n");
    exit(EXIT_FAILURE);
  }

  if (!queue_is_empty(g->redo)) {
    element *action = (element *)queue_peek_tail(g->redo);
    queue_pop_tail(g->redo);

    color couleur_transfer = game_get_color(g, action->i, action->j);
    element *replace = malloc(sizeof(element));
    replace->i = action->i;
    replace->j = action->j;
    replace->couleur = couleur_transfer;
    queue_push_tail(g->undo, replace);

    game_set_color(g, action->i, action->j, action->couleur);
    free(action);
    //  Pas besoin de libérer la mémoire car on manipule les éléments dans la
    //  file
  }
}
