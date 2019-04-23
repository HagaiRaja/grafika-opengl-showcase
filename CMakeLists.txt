cmake_minimum_required(VERSION 3.13)
project(grafika-opengl-showcase)

set(CMAKE_CXX_STANDARD 14)

add_executable(grafika-opengl-showcase lightning_sample.cpp glad.c)

set(SOURCE_FILES glad.c lightning_sample.cpp)
target_link_libraries(grafika-opengl-showcase GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor assimp --enable-nuklear)