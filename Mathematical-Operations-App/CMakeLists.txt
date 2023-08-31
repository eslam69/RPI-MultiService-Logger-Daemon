cmake_minimum_required(VERSION 3.10)

project(mathApp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER /home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-g++)

# Add the IPC-Library include directory
include_directories(../../IPC-Library/include)

# Add the IPC-Library library
link_directories(../../IPC-Library/build)

# Add the math_lib executable
add_executable(math src/math_lib.cpp)
find_library(MSGQCONNECT_LIB MsgQConnect PATHS ../../IPC-Library/build)

# Link against the MsgQConnect library
target_link_libraries(math  ${MSGQCONNECT_LIB} rt pthread)