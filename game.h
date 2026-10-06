
#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

typedef unsigned int uint;

#define DEFAULT_SIZE 5

typedef enum { EMPTY, WHITE, BLACK } color;

typedef enum {
  UNCONSTRAINED = -1,
  MIN_CONSTRAINT = -1,
  MAX_CONSTRAINT = 9
} constraint;

typedef enum { ERROR, UNSATISFIED, SATISFIED } status;

typedef enum {
  HERE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT
} direction;

typedef struct game_s* game;

typedef const struct game_s* cgame;

game game_new(constraint* constraints, color* colors);
/*
  Creates a new game, dynamically allocate, pointed by a "newGame" variable
  Give it the default size (DEFAULT_SIZE), wrapping (FALSE) and neighbourhood
  (FULL) Initializes tiles with arrays arguments "constraints" and "colors" Only
  initialize tiles with "EMPTY" if colors array is NULL This fonction use
  game_new_empty() (read it for more infos).
  */

game game_new_empty(void);
/*
  Creates a new game, dynamically allocate, pointed by a "newGame" variable
  Also allocate 2 arrays member of the struct, temporarily pointed in
  arrayOfConstraint and arrayOfColors var Give it the default size
  (DEFAULT_SIZE), wrapping (FALSE) and neighbourhood (FULL) Initializes every
  tiles with EMPTY colors and UNCONSTRAINT constraint This function use
  queue_new() of queue lib, to initialize undo and redo member
  */

game game_copy(cgame g);
/*
  Take a cgame variable g and create a exact copy of this game, including the
  number of rows, collums, all the constaints and colors, the wapping and
  neighbourhood Type settings.
  */

bool game_equal(cgame g1, cgame g2);
/*
  Test if 2 game pointed by g1 and g2 are equals
  Games need to have the same number of colones, Rows, neighbourhoodType but not
  the same wrapping This fonction test every tiles of both games and test if
  they have the same color, constraint and status This function use get_color,
  get constraint and get_status fonctions
  */

void game_delete(game g);
/*
  Free the game g and every suballowed member of the struct
  Never exit even if g is NULL
  */

void game_set_constraint(game g, uint i, uint j, constraint n);
/*
  Set the tile of position i/j with the constraint c of the game g
  i/j must be contain in the game size
  Use the function game_nb_cols
  */

color game_get_color(cgame g, uint i, uint j);
/*
  Set the tile of position i/j with the color c of the game g
  i/j must be contain in the game size
  Use the function game_nb_cols
  */

void game_set_color(game g, uint i, uint j, color c);
/*
  Return the color of the tile of position i/j of the game g
  i/j must be in the game size
  Use the function game_nb_cols
  Can return 3 values : WHITE , BLACK , EMPTY
  */

constraint game_get_constraint(cgame g, uint i, uint j);
/*
  Return the constraint of the tile of position i/j of the game g
  i/j must be contain in the game size
  Can return either the constraint values or UNCONSTRAINED
  */

bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint* pi_next,
                          uint* pj_next);
/*
  Return true if the square next to the i/j square,in the choosen direction, if
  in the grid, return false otherwise.
  Use the function game_nb_cols and game_nb_rows
  */
status game_get_status(cgame g, uint i, uint j);
/*
  Return the status of the tile of position i/j of the game g
  i/j must be in the game size
  Can return 3 values : ERROR , UNSATISFIED , SATISFIED
  */

int game_nb_neighbors(cgame g, uint i, uint j, color c);
/*
  Returns the number of neighbors of the same color.

  This function counts the number of squares of color c in the 3x3 neighborhood
  of the square at coordinate (i, j), including the square itself in this count.
*/

void game_play_move(game g, uint i, uint j, color c);
/*
  Plays a move in a given square. With given indices i and j, and the game and
  color wanted.
  */

bool game_won(cgame g);
/*
  Return a boolean, it can be either true if all constraints are valid and all
  squares colored, or false if these conditions aren't valid.
  This fonction use game_nb_rows and game_nb_cols
  */

void game_restart(game g);
/*
  Restart the game.
  Resetting colors with the EMPTY value.
  */

#endif  // __GAME_H__