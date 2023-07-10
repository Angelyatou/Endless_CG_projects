cmake_minimum_required(VERSION 3.15)
project(main)

add_executable(main main.cpp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

