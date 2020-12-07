cmake_minimum_required(VERSION 3.17)
project(WASM_mini_laska C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall")
set("TERM=xterm-256color")

add_executable(WASM_mini_laska lib/util/list.c lib/board.c lib/control.c lib/brain.c lib/game.c main.c lib/engine.c)
target_link_libraries(WASM_mini_laska m)