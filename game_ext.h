
#ifndef __GAME_EXT_H__
#define __GAME_EXT_H__

#include <stdbool.h>

#include "game.h"

typedef enum { FULL, ORTHO, FULL_EXCLUDE, ORTHO_EXCLUDE } neighbourhood;

game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints,
                  color *colors, bool wrapping, neighbourhood neigh);

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        neighbourhood neigh);

uint game_nb_rows(cgame g);

uint game_nb_cols(cgame g);

bool game_is_wrapping(cgame g);

neighbourhood game_get_neighbourhood(cgame g);

void game_undo(game g);

void game_redo(game g);

#endif  // __GAME_EXT_H__