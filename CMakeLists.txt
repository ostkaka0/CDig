project(CDig)
cmake_minimum_required(VERSION 2.8)
file(GLOB_RECURSE SRC_LIST "src/*.c" "src/*.h")
add_executable(${PROJECT_NAME} ${SRC_LIST})
SET(CMAKE_C_FLAGS "-std=c11 -lm")
