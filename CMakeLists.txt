cmake_minimum_required(VERSION 3.0)
project(game_text C)
set(CMAKE_BUILD_TYPE=Debug)
include(CTest)
enable_testing()

link_directories(${CMAKE_SOURCE_DIR})

## find SDL2
include(sdl2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

# file copy for game_load test
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_save_1.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_save_2.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_save_3.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_save_4.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_save_5.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_save_6.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_save_7.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_save_8.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


set(CMAKE_C_FLAGS "-Wall -std=c99")            # options de compilation génériques
set(CMAKE_C_FLAGS_DEBUG "-g -O0 --coverage")   # options de compilation en mode Debug
set(CMAKE_C_FLAGS_RELEASE "-O3")               # options de compilation en mode Release

add_library(game game.c game_aux.c game_ext.c queue.c game_tools.c)
add_library(game2 game.c game_aux.c game_ext.c queue.c game_tools.c)  

add_library(queue queue.c)

add_executable(game_text game_text.c)

target_link_libraries(game_text game)

add_executable(game_solve game_solve.c)
target_link_libraries(game_solve game2)

add_executable(game_test_eraze game_test_eraze.c)
target_compile_definitions(game_test_eraze PRIVATE -DNDEBUG)

add_executable(game_test_yblothiaux game_test_yblothiaux.c)
target_compile_definitions(game_test_yblothiaux PRIVATE -DNDEBUG)

add_executable(game_test_lbettcher game_test_lbettcher.c)
target_compile_definitions(game_test_lbettcher PRIVATE -DNDEBUG)

add_executable(game_test_ext game_test_ext.c)
target_compile_definitions(game_test_ext PRIVATE -DNDEBUG)

add_executable(game_test_tools game_test_tools.c)
target_compile_definitions(game_test_tools PRIVATE -DNDEBUG)


#SDL2 exe
include_directories(${SDL2_LIBRARIES})
add_executable(game_sdl main.c model.c)
target_link_libraries(game_sdl game ${SDL2_ALL_LIBS} m)


######## TEST ############ 

target_link_libraries(game_test_eraze game)

target_link_libraries(game_test_lbettcher game)

target_link_libraries(game_test_yblothiaux game)

target_link_libraries(game_test_ext game)

target_link_libraries(game_test_tools game)

add_test(test_eraze_dummy ./game_test_eraze dummy)
add_test(test_eraze_game_play_move ./game_test_eraze game_play_move)
add_test(test_eraze_game_get_next_square ./game_test_eraze game_get_next_square)
add_test(test_eraze_game_set_color ./game_test_eraze game_set_color)
add_test(test_eraze_game_delete ./game_test_eraze game_delete)
add_test(test_eraze_game_print ./game_test_eraze game_print)
add_test(test_eraze_game_new ./game_test_eraze game_new)

add_test(test_yblothiaux_dummy ./game_test_yblothiaux dummy)
add_test(test_yblothiaux_game_copy ./game_test_yblothiaux game_copy)
add_test(test_yblothiaux_game_equal ./game_test_yblothiaux game_equal)
add_test(test_yblothiaux_game_default_solution ./game_test_yblothiaux game_default_solution)
add_test(test_yblothiaux_game_get_color ./game_test_yblothiaux game_get_color)
add_test(test_yblothiaux_game_nb_neighbors ./game_test_yblothiaux game_nb_neighbors)
add_test(test_yblothiaux_game_restart ./game_test_yblothiaux game_restart)

add_test(test_lbettcher_dummy ./game_test_lbettcher dummy)
add_test(test_lbettcher_game_new_empty ./game_test_lbettcher game_new_empty)
add_test(test_lbettcher_game_default ./game_test_lbettcher game_default)
add_test(test_lbettcher_game_get_status ./game_test_lbettcher game_get_status)
add_test(test_lbettcher_game_get_constraint ./game_test_lbettcher game_get_constraint)
add_test(test_lbettcher_game_set_constraint ./game_test_lbettcher game_set_constraint)
add_test(test_lbettcher_game_won ./game_test_lbettcher game_won)

add_test(test_game_new_ext ./game_test_ext game_new_ext)
add_test(test_game_new_empty_ext ./game_test_ext game_new_empty_ext)
add_test(test_game_nb_rows ./game_test_ext game_nb_rows)
add_test(test_game_nb_cols ./game_test_ext game_nb_cols)
add_test(test_game_is_wrapping ./game_test_ext game_is_wrapping)
add_test(test_game_get_neighbourhood ./game_test_ext game_get_neighbourhood)
add_test(test_game_undo ./game_test_ext game_undo)
add_test(test_game_redo ./game_test_ext game_redo)

add_test(test_game_save ./game_test_tools game_save)
add_test(test_game_load ./game_test_tools game_load)