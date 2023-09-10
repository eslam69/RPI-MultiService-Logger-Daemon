


cmake_minimum_required(VERSION 3.10)

project(LoggingDaemon)

set(CMAKE_CXX_STANDARD 11)

# Specify the path to your cross-compiler toolchain file
set(CMAKE_C_COMPILER /home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-g++)

# Add the IPC-Library include directory
include_directories(../../IPC-Library/include)

# Add the Boost include directory
include_directories(/usr/include/boost)

# Add the daemon executable
add_executable(daemon src/daemon.cpp)
set(CMAKE_PREFIX_PATH "/home/eslam/cross_compiled_boost/lib")
# Find the Boost libraries
find_package(Boost 1.80.0 REQUIRED COMPONENTS log log_setup system thread)
# set(Boost_LIBRARIES /usr/lib/libboost_log.so /usr/lib/libboost_log_setup.so /usr/lib/libboost_system.so /usr/lib/libboost_thread.so)
#print Boost_INCLUDE_DIRS
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
# Link against the MsgQConnect, Boost, and other required libraries
find_library(MSGQCONNECT_LIB MsgQConnect PATHS ../../IPC-Library/build)

target_link_libraries(daemon ${MSGQCONNECT_LIB} rt pthread Boost::log Boost::log_setup Boost::system Boost::thread)
