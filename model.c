// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "model.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // required to load transparent texture from PNG
#include <SDL2/SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

#define FONT "../res/minecraft_final.ttf"
#define FONTSIZE 196
#define BACKGROUND "../res/fond2.png"
#define CASEWHITE "../res/white_block.png"
#define CASEBLACK "../res/case_noire.png"
#define CASEEMPTY "../res/rien.png"
#define BUTTON_SAVE "../res/save3.png"
#define BUTTON_REDO "../res/redo.png"
#define BUTTON_UNDO "../res/undo.png"
#define BUTTON_RESTART "../res/restart2.png"
#define BUTTON_SOLVE "../res/solve.png"

// Number of action buttons use in the program
#define NB_BUTTON 5

// Set size of random generate game (if save not used)
#define MIN_SIZE_RANDOM 2
#define MAX_SIZE_RANDOM 5

// Name of the save file name used if no save file given
#define SAVE_FILE_DEFAULT "temp"

/* **************************************************************** */

struct Env_t {
  SDL_Texture *background;      // Texture of the background
  SDL_Texture *white;           // Texture of whitesquareof the game
  SDL_Texture *black;           // Texture of black square of the game
  SDL_Texture *empty;           // Texture of empty square of the game
  SDL_Texture *button_save;     // Texture of button save square of the game
  SDL_Texture *button_redo;     // Texture of button redo square of the game
  SDL_Texture *button_undo;     // Texture of button undo square of the game
  SDL_Texture *button_restart;  // Texture of button restart square of the game
  SDL_Texture *button_solve;    // Texture of button solve square of the game

  game gameInstance;      // Game use in the app
  SDL_Rect *rectArray;    // Array of Rect, one Rect is one square of the game
  SDL_Rect *buttonArray;  // Undo/Redo/Restart/Save/Solve
  char *filePath;         // Path to the file of loaded game or NULL if default
};

// Free env parameter and print message (mainly use for allocation error)
void freeEnv(Env *env, char *message) {
  if (message != NULL) {
    fprintf(stderr, "%s", message);
  }
  game_delete(env->gameInstance);
  free(env->rectArray);
  free(env->filePath);
  free(env->buttonArray);
  free(env);
}

/* **************************************************************** */

void resize(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  // Get the window size
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // Get the game size
  int wGame = game_nb_cols(env->gameInstance);
  int hGame = game_nb_rows(env->gameInstance);

  // Calculate padding
  int padding = fmin(w, h) * 0.15;

  // Calculate block size based on available space and number of cells
  int blockSize = fmin((w - 2 * padding) / wGame, (h - 2 * padding) / hGame);

  // Limit block size to prevent overflow
  int maxBlockSize = fmin(w / wGame, h / hGame * 0.6);
  blockSize = fmin(blockSize, maxBlockSize);

  // Calculate actual total size of the game grid
  int totalWidth = wGame * blockSize;
  int totalHeight = hGame * blockSize;

  // Calculate starting position to center the grid
  int startX = (w - totalWidth) / 2;
  int startY =
      (h - totalHeight) / 2 + padding / 2;  // Add padding at the bottom

  // Renew each game Rect, giving it its position and size
  int x = startX;
  int y = startY;
  for (int i = 0; i < wGame * hGame; i++) {
    env->rectArray[i].h = blockSize;
    env->rectArray[i].w = blockSize;
    env->rectArray[i].x = x;
    env->rectArray[i].y = y;
    x += blockSize;
    if (x >= startX + totalWidth) {
      x = startX;
      y += blockSize;
    }
  }

  // Renew each button Rect, giving it its position and size
  int buttonWidth = w / (NB_BUTTON * 2 + 1);
  x = buttonWidth;
  y = h * 0.1;
  for (int i = 0; i < NB_BUTTON; i++) {
    env->buttonArray[i].h = buttonWidth;
    env->buttonArray[i].w = buttonWidth;
    env->buttonArray[i].x = x - buttonWidth / 2;
    env->buttonArray[i].y = y;
    x += 2 * buttonWidth;
  }
}

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  // Init each textures
  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);
  env->white = IMG_LoadTexture(ren, CASEWHITE);
  if (!env->white) ERROR("IMG_LoadTexture: %s\n", CASEWHITE);
  env->black = IMG_LoadTexture(ren, CASEBLACK);
  if (!env->black) ERROR("IMG_LoadTexture: %s\n", CASEBLACK);
  env->empty = IMG_LoadTexture(ren, CASEEMPTY);
  if (!env->empty) ERROR("IMG_LoadTexture: %s\n", CASEEMPTY);

  env->button_save = IMG_LoadTexture(ren, BUTTON_SAVE);
  if (!env->button_save) ERROR("IMG_LoadTexture: %s\n", BUTTON_SAVE);
  env->button_redo = IMG_LoadTexture(ren, BUTTON_REDO);
  if (!env->button_redo) ERROR("IMG_LoadTexture: %s\n", BUTTON_REDO);
  env->button_undo = IMG_LoadTexture(ren, BUTTON_UNDO);
  if (!env->button_undo) ERROR("IMG_LoadTexture: %s\n", BUTTON_UNDO);
  env->button_restart = IMG_LoadTexture(ren, BUTTON_RESTART);
  if (!env->button_restart) ERROR("IMG_LoadTexture: %s\n", BUTTON_RESTART);
  env->button_solve = IMG_LoadTexture(ren, BUTTON_SOLVE);
  if (!env->button_solve) ERROR("IMG_LoadTexture: %s\n", BUTTON_SOLVE);

  // Load given game save file (or random if none), and save the file path (or
  //  if none)
  game g;
  if (argc == 2) {
    g = game_load(argv[1]);
    if (g == NULL) {
      freeEnv(env, "Erreur en chargeant le fichier game");
      exit(EXIT_FAILURE);
    }
    env->filePath = malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(env->filePath, argv[1]);
  } else {
    srand(time(NULL));
    // Generate a random size with size between MIN_SIZE_RANDOM and
    // MAX_SIZE_RANDOM and save it on the nale SAVE_FILE_DEFAULT for restart
    // usage
    g = game_random(
        rand() % ((MAX_SIZE_RANDOM + 1) - MIN_SIZE_RANDOM) + MIN_SIZE_RANDOM,
        rand() % ((MAX_SIZE_RANDOM + 1) - MIN_SIZE_RANDOM) + MIN_SIZE_RANDOM,
        false, 0, false, 0.5, 0.5);
    game_save(g, SAVE_FILE_DEFAULT);
    env->filePath = malloc((strlen(SAVE_FILE_DEFAULT) + 1) * sizeof(char));
    (strcpy(env->filePath, SAVE_FILE_DEFAULT));
  }

  env->gameInstance = g;

  // Get the game size
  int wGame = game_nb_cols(env->gameInstance);
  int hGame = game_nb_rows(env->gameInstance);

  // Allocate Rect array
  env->rectArray = malloc(wGame * hGame * sizeof(SDL_Rect));
  if (env->rectArray == NULL) {
    freeEnv(env, "SDL_Rect game array array malloc error\n");
    exit(EXIT_FAILURE);
  }

  // Allocate Button array
  env->buttonArray = malloc(NB_BUTTON * sizeof(SDL_Rect));
  if (env->buttonArray == NULL) {
    freeEnv(env, "SDL_Rect button array malloc error\n");
    exit(EXIT_FAILURE);
  }

  resize(win, ren, env);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  // Get the game size
  int wGame = game_nb_cols(env->gameInstance);
  int hGame = game_nb_rows(env->gameInstance);

  // Render the Background
  SDL_RenderCopy(ren, env->background, NULL, NULL);

  /* init text texture using Arial font */
  SDL_Color w_color = {255, 255, 255, 255}; /* white color in RGBA */
  SDL_Color g_color = {62, 244, 214, 255};  /* green color in RGBA */
  SDL_Color r_color = {255, 0, 0, 255};     /* red color in RGBA */
  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font) ERROR("TTF_OpenFont: %s\n", FONT);

  resize(win, ren, env);

  // Browse the game and render each square, depending of his color/number
  for (int x = 0; x < hGame; x++) {
    for (int y = 0; y < wGame; y++) {
      int i = x * wGame + y;
      switch (game_get_color(env->gameInstance, x, y)) {
        case WHITE:
          SDL_RenderCopy(ren, env->white, NULL, &(env->rectArray[i]));
          break;
        case BLACK:
          SDL_RenderCopy(ren, env->black, NULL, &(env->rectArray[i]));
          break;
        case EMPTY:
          SDL_RenderCopy(ren, env->empty, NULL, &(env->rectArray[i]));
          break;
      }
      // Render the constraint of each square
      int constraint = game_get_constraint(env->gameInstance, x, y);
      if (constraint >= 0) {
        char str[20];
        sprintf(str, "%d", constraint);
        SDL_Surface *surf;
        if (game_get_status(env->gameInstance, x, y) == SATISFIED) {
          surf = TTF_RenderText_Blended(
              font, str, g_color);  // blended rendering for ultra nice text
        } else if (game_get_status(env->gameInstance, x, y) == UNSATISFIED) {
          surf = TTF_RenderText_Blended(
              font, str, w_color);  // blended rendering for ultra nice text
        } else {
          surf = TTF_RenderText_Blended(
              font, str, r_color);  // blended rendering for ultra nice text
        }
        SDL_RenderCopy(ren, SDL_CreateTextureFromSurface(ren, surf), NULL,
                       &(env->rectArray[i]));
        SDL_FreeSurface(surf);
      }
    }
  }
  TTF_CloseFont(font);

  // Render button rect array (TODO)
  for (int i = 0; i < NB_BUTTON; i++) {
    switch (i) {
      case 0:
        SDL_RenderCopy(ren, env->button_undo, NULL, &(env->buttonArray[i]));
        break;
      case 1:
        SDL_RenderCopy(ren, env->button_redo, NULL, &(env->buttonArray[i]));
        break;
      case 2:
        SDL_RenderCopy(ren, env->button_restart, NULL, &(env->buttonArray[i]));
        break;
      case 3:
        SDL_RenderCopy(ren, env->button_save, NULL, &(env->buttonArray[i]));
        break;
      case 4:
        SDL_RenderCopy(ren, env->button_solve, NULL, &(env->buttonArray[i]));
        break;
    }
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  // Get the game size
  int wGame = game_nb_cols(env->gameInstance);
  int hGame = game_nb_rows(env->gameInstance);

  switch (e->type) {
    // Quit the program event
    case (SDL_QUIT):
      return true;
      break;

    // Bind escape key to quit the program
    case (SDL_KEYDOWN):
    case SDLK_ESCAPE:
      return true;
      break;

    // Click on the program event
    case (SDL_MOUSEBUTTONDOWN): {
      SDL_Point mouse = {0, 0};
      SDL_GetMouseState(&mouse.x, &mouse.y);

      // Check if there is click on each square of rendered game rect, and play
      // a move
      for (int x = 0; x < hGame; x++) {
        for (int y = 0; y < wGame; y++) {
          int i = x * wGame + y;
          if (SDL_PointInRect(&mouse, &(env->rectArray[i])) == true) {
            switch (e->button.button) {
              case SDL_BUTTON_LEFT:
                game_play_move(env->gameInstance, x, y, BLACK);
                break;

              case SDL_BUTTON_RIGHT:
                game_play_move(env->gameInstance, x, y, WHITE);
                break;
            }
          }
        }
      }

      // Check if there is click on each square of rendered button rect, and do
      // the action linked
      for (int i = 0; i < NB_BUTTON; i++) {
        if (SDL_PointInRect(&mouse, &(env->buttonArray[i])) == true) {
          switch (i) {
            case 0:
              game_undo(env->gameInstance);
              break;
            case 1:
              game_redo(env->gameInstance);
              break;
            case 2:
              // restart
              env->gameInstance = game_load(env->filePath);
              break;
            case 3:
              // save
              game_save(env->gameInstance, env->filePath);
              break;
            case 4:
              // solve
              game_solve(env->gameInstance);
              break;
          }
        }
      }
    }
  }
  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->white);
  SDL_DestroyTexture(env->black);
  SDL_DestroyTexture(env->empty);
  SDL_DestroyTexture(env->button_redo);
  SDL_DestroyTexture(env->button_undo);
  SDL_DestroyTexture(env->button_restart);
  SDL_DestroyTexture(env->button_save);
  SDL_DestroyTexture(env->button_solve);
  freeEnv(env, "Fin du programme\n");
}

/* **************************************************************** */