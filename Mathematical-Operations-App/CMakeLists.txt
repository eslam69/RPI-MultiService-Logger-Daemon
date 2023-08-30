cmake_minimum_required(VERSION 3.10)

project(mathApp)

set(CMAKE_CXX_STANDARD 11)

# Add the IPC-Library include directory
include_directories(../../IPC-Library/include)

# Add the IPC-Library library
# link_directories(../../IPC-Library/lib)

# Add the math_lib executable
add_executable(math src/math_lib.cpp)

# Link against the MsgQConnect library
target_link_libraries(math MsgQConnect rt pthread)