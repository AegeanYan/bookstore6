
cmake_minimum_required(VERSION 3.10.2)
project(code)
set(CMAKE_CXX_STANDARD 14)
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
include_directories(
        ${PROJECT_SOURCE_DIR}/src/

)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
add_executable(code src/main.cpp src/Bookstore.cpp src/Bookstore.h src/Block_linked_list.cpp src/Block_linked_list.h src/Users.cpp src/Users.h)
