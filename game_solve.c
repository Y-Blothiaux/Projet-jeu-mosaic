#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Wrong number of parameter\n");
    return EXIT_SUCCESS;
  }
  if (argv[1][0] == '-' && argv[1][1] == 's') {
    game g = game_load(argv[2]);
    if (game_solve(g) == false) {
      return EXIT_SUCCESS;
    }
    if (argc == 4) {
      game_save(g, argv[3]);
    } else {
      game_print(g);
      printf("\n");
    }

  } else if (argv[1][0] == '-' && argv[1][1] == 'c') {
    game g = game_load(argv[2]);
    if (argc == 4) {
      FILE* fichier = fopen(argv[3], "w");
      fprintf(fichier, "%d\n", game_nb_solutions(g));
      fclose(fichier);
    } else {
      printf("%d", game_nb_solutions(g));
      printf("\n");
    }
    return EXIT_SUCCESS;
  }
}