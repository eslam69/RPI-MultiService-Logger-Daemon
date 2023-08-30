cmake_minimum_required(VERSION 3.10)

project(LoggingDaemon)

set(CMAKE_CXX_STANDARD 11)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# Add the IPC-Library include directory
include_directories(../../IPC-Library/include)

# Add the IPC-Library library
link_directories(../../IPC-Library/lib)

# Add the Boost libraries
# find_package(Boost REQUIRED COMPONENTS log log_setup system thread)
find_package(Boost 1.80.0 REQUIRED COMPONENTS log log_setup system thread)
set(Boost_LIBRARIES Boost::log Boost::log_setup Boost::system Boost::thread)
# Add the daemon executable
add_executable(daemon src/daemon.cpp)

# Link against the MsgQConnect and Boost libraries
# target_link_libraries(daemon MsgQConnect rt pthread Boost::log Boost::log_setup Boost::system Boost::thread)



target_link_libraries(daemon MsgQConnect rt pthread ${Boost_LIBRARIES})