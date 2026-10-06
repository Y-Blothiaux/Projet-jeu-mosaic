#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"
#include "queue.h"

void print_help() {
  printf("- press 'w <i> <j>' to set square (i,j) white\n");
  printf("- press 'b <i> <j>' to set square (i,j) black\n");
  printf("- press 'e <i> <j>' to set square (i,j) empty\n");
  printf("- press 'z' to undo\n");
  printf("- press 'y' to redo\n");
  printf("- press 'r' to restart\n");
  printf("- press 'q' to quit\n");
  printf("- press 's' to save\n");
}

void print_error(game g) {
  for (int ln = 0; ln < game_nb_rows(g); ln++) {
    for (int cl = 0; cl < game_nb_cols(g); cl++) {
      if (game_get_constraint(g, ln, cl) != -1) {
        char stat = game_get_status(g, ln, cl);
        if (stat == ERROR) {
          printf("Square (%d, %d): error\n", ln, cl);
        } else if (stat == UNSATISFIED) {
          printf("Square (%d, %d): unsatisfied\n", ln, cl);
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  game g;
  if (argc == 2) {
    g = game_load(argv[1]);
    if (g == NULL) {
      fprintf(stderr, "Erreur en chargeant %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  } else {
    g = game_default();
  }
  char userInput;
  char file[100];
  int i, j, c;
  game_print(g);

  while (game_won(g) == false) {
    printf("\n");
    printf("? [<h> for help]\n>");
    if (scanf(" %c", &userInput) == EOF) {
      printf("can't read text input\n");
    }
    printf("\n");

    if (userInput == 'h' || userInput == 'H') {
      printf("action : help\n");
      print_help();
    } else if (userInput == 'z' || userInput == 'Z') {
      printf("action : undo\n");
      game_undo(g);
    } else if (userInput == 'y' || userInput == 'Y') {
      printf("action : redo\n");
      game_redo(g);
    } else if (userInput == 's' || userInput == 'S') {
      printf("action : save\n");
      if (scanf("%s", file) == EOF) {
        printf("cant read safe file\n");
      }
      game_save(g, file);
    } else if (userInput == 'r' || userInput == 'R') {
      printf("action : restart\n");
      game_restart(g);
    } else if (userInput == 'w' || userInput == 'W') {
      if (scanf("%d %d", &i, &j) == EOF) {
        printf("cant read move data\n");
      }
      if (i >= game_nb_rows(g) || i < 0 || j < 0 || j >= game_nb_cols(g)) {
        printf("action out of the grid - ERROR\n");
        game_delete(g);
        return EXIT_SUCCESS;
      }
      printf("action: play move 'w' in square (%d,%d)\n", i, j);
      game_play_move(g, i, j, WHITE);
    } else if (userInput == 'b' || userInput == 'B') {
      if (scanf("%d %d", &i, &j) == EOF) {
        printf("cant read move data\n");
      }
      if (i >= game_nb_rows(g) || i < 0 || j < 0 || j >= game_nb_cols(g)) {
        printf("action out of the grid - ERROR\n");
        game_delete(g);
        return EXIT_SUCCESS;
      }
      printf("action: play move 'b' in square (%d,%d)\n", i, j);
      game_play_move(g, i, j, BLACK);
    } else if (userInput == 'e' || userInput == 'E') {
      if (scanf("%d %d", &i, &j) == EOF) {
        printf("cant read move data\n");
      }
      if (i >= game_nb_rows(g) || i < 0 || j < 0 || j >= game_nb_cols(g)) {
        printf("action out of the grid - ERROR\n");
        game_delete(g);
        return EXIT_SUCCESS;
      }
      printf("action: play move 'e' in square (%d,%d)\n", i, j);
      game_play_move(g, i, j, EMPTY);
    } else if (userInput == 'q' || userInput == 'Q') {
      printf("What a shame, you gave up :-(\n");
      game_delete(g);
      return EXIT_SUCCESS;
    }
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    game_print(g);
    printf("\n");
    print_error(g);
  }

  printf("Congratulation, you win\n");
  game_print(g);
  game_delete(g);
  return EXIT_SUCCESS;
}