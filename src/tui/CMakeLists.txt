# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

# Set the project name
project(status_tui)

# Specify the C standard
set(CMAKE_C_STANDARD 99)

# Add the executable
add_executable(status_tui
    tui_renderer.c
    keys_parser.c
)

# Link the ncurses library
target_link_libraries(status_tui PRIVATE ncurses)
