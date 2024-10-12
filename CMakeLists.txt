cmake_minimum_required(VERSION 3.13)
project(proyecto_a)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_executable(proyecto_a ${SOURCE_FILES})
