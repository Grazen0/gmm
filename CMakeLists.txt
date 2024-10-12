cmake_minimum_required(VERSION 3.13)
project(proyecto_a)
set(CMAKE_CXX_STANDARD 20)

# https://stackoverflow.com/questions/3201154/automatically-add-all-files-in-a-folder-to-a-target-using-cmake
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_executable(proyecto_a ${SOURCE_FILES})
