#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_struct.h"

void game_print(cgame g) {
  /*
  Print the game g in argument
  */
  if (g == NULL) {
    fprintf(stderr, "Mauvais argument\n");
    return;
  }

  char* wtab[] = {"🄋", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈"};
  char* btab[] = {"🄌", "➊", "➋", "➌", "➍", "➎", "➏", "➐", "➑", "➒"};
  char* squares[] = {"□", "■"};

  for (int i = 0; i < game_nb_cols(g); i++) {
    printf(" %d", i);
  }
  printf("\n   ");

  for (int i = 0; i < game_nb_cols(g) * 2; i++) {
    printf("-");
  }
  printf("\n");

  for (int i = 0; i < game_nb_rows(g); i++) {
    printf("%d |", i);
    for (int j = 0; j < game_nb_cols(g); j++) {
      int val = game_get_constraint(g, i, j);

      if (val != -1) {
        color c = game_get_color(g, i, j);
        if (c == BLACK) {
          printf("%s ", btab[val]);
        } else if (c == WHITE) {
          printf("%s ", wtab[val]);
        } else {
          printf("%d ", val);
        }
      } else {
        color c = game_get_color(g, i, j);
        if (c == WHITE) {
          printf("%s ", squares[0]);
        } else if (c == BLACK) {
          printf("%s ", squares[1]);
        } else {
          printf("  ");
        }
      }
    }
    printf("|\n");
  }

  printf("   ");
  for (int i = 0; i < game_nb_cols(g) * 2; i++) {
    printf("-");
  }
}

/*
Return the default game (see in the documentation)
Create 2 arrays "constraints" and "colors" who contains these colors and
constraints Use game_new fonction
*/
game game_default(void) {
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
  game newgame = game_new(constraints, colors);
  return newgame;
}

game game_default_solution(void) {
  game g = game_default();
  color colorS[25] = {WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
                      BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE,
                      WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK,
                      BLACK, BLACK, BLACK, WHITE};
  for (int i = 0; i < g->nbRows; i++) {
    for (int j = 0; j < g->nbCols; j++) {
      game_set_color(g, i, j, colorS[(i * g->nbRows) + j]);
    }
  }
  return g;
}
