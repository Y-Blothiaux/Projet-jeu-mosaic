#ifndef __STRUCT_H__
#define __STRUCT_H__

#include "game.h"
#include "game_ext.h"
#include "queue.h"

/*
To represent the game with this struct, we use theses member:
- 2 array of constraints/colors "constraints" and "colors" to represent every
tiles
- 2 pointers to queue type for "undo" and "redo" actions
- A bool type "isWrapping" to represent the wrapping of the game
- A neighbourhood type, the one of the game
- 2 uint "nbRows" and "nbCols" for the size of the game
*/
struct game_s {
  constraint *constraints;
  color *colors;
  queue *undo;
  queue *redo;
  bool isWrapping;
  neighbourhood neighbourhoodType;
  uint nbRows;
  uint nbCols;
};

typedef struct element {
  int i;
  int j;
  color couleur;
} element;

#endif /*STRUCT_H*/