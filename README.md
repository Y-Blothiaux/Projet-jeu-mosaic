# mosaic-a19
Redacted and coded by Erwan RAZE, Leonard BETTCHER & Yanis BLOTHIAUX, Group A19

Project coded in C for the second year class of "Projet Technologique" at the University of Bordeaux, 2023/2024



---------------
The Game itslef
---------------

According to the Simon Tatham's Portable Puzzle Collection:

    ‍Given a grid of squares, you must color it either black or white. Some squares contain clue numbers. Each clue tells you the number of black squares in the 3×3 region surrounding the clue - including the clue square itself.

So, to play this game, you have to choose the white or black color of each square to fill in all the grid. For example, if an empty cell contains the number 0, then the 3x3 neighbourhood must be white. Similarly, if a square contains the number 9, the 3x3 neighbourhood will be black.

For more info:
https://pt2.gitlabpages.inria.fr/support/doc/v1/html/




---------------
How to play
---------------

You can play BLACK or WHITE by using the right of left click of your mouse,
When the constraint is fulfilled it will turn to green, if it's wrong it'll be red, meaning you have too many BLACK around it.

The 5 buttons on top allows you to respectively, undo a play, redo it, restart the game, save it and finally solve the game your playing.




---------------
About the coding
---------------

To use the code :
- create a build file : mkdir build
- enter it : cd build
- type the following lines of code :
    cmake ..
    make

Now you have several options :
- ./game_sdl                to create a random game and play it
- ./game_sdl name           to play your game "name"
- ./game_text               to play the default game in the terminal
- ./game_text name          to play your game "name" in the terminal