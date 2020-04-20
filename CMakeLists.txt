cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -Weverything")

project(lk_array_test)

add_executable(${CMAKE_PROJECT_NAME} main.c
    lk_array.c
)

target_link_libraries(${CMAKE_PROJECT_NAME} m)
