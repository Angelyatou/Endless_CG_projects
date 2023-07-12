cmake_minimum_required(VERSION 3.15)
project(main)

add_executable(main draw_polygon/draw_triangle/main.cpp draw_polygon/draw_triangle/ShaderSource.h draw_polygon/draw_triangle/VertexData.h template/depends/glad/glad.cpp template/depends/glad/glad.h)

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(GLUT REQUIRED)
target_link_libraries(main PRIVATE GLUT::GLUT)
target_link_libraries(main PRIVATE opengl32)
